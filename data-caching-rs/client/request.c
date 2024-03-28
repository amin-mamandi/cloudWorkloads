
#include "request.h"
#ifdef GEM5
#include "m5op.h"
#endif

void sendRequest(struct request* request) {

  struct request* sendRequest = request;
  unixSocketSendRequest(sendRequest);

}//End sendRequest()

void unixSocketSendRequest(struct request* request) {
    // Your logic for sending requests over Unix sockets
    // Example:
    // printf("Unix Socket\n");
    int totalSize = request->value_size + request->key_size + request->header.extras_length + sizeof(struct request_header);

    char* oneBigPacket = malloc(sizeof(char) * totalSize);
    char* ptr = oneBigPacket;

    memcpy(ptr, (char*)&request->header, sizeof(struct request_header));
    ptr += sizeof(struct request_header);

    memcpy(ptr, request->extras, request->header.extras_length);
    ptr += request->header.extras_length;

    memcpy(ptr, request->key, request->key_size);
    ptr += request->key_size;

    memcpy(ptr, request->value, request->value_size);

    gettimeofday(&request->send_time, NULL);
    writeBlock(request->connection->sock, oneBigPacket, totalSize);
    // printf("Sent request Done\n");
    free(oneBigPacket);
    
}
  

void deleteRequest(struct request* request) {

  struct request* currentRequest = request;
  while(currentRequest != NULL) {

    if(currentRequest->value != NULL){
      free(currentRequest->value);
    }

    if(currentRequest->extras != NULL){
      free(currentRequest->extras);
    } 


    struct request* nextRequest;
    nextRequest = currentRequest->next_request;
    free(currentRequest);
    currentRequest = nextRequest;

  }//End while

}//End deleteRequest()

int generateUID(struct worker* worker) {

  struct config* config = worker->config;

  uint32_t uid = __sync_fetch_and_add(&(config->current_request_uid), 1);
  
  return uid;

}


struct request* createRequest(int requestType, struct conn* conn, struct worker* worker, char* key, char* value, int type) {

  struct request* request = malloc(sizeof(struct request));
  request->worker = worker;
  request->bad_multiget = 0;

  if(conn == NULL){
    printf("Tried to give request a null connection\n");
    exit(-1);
  }
  request->connection = conn;

  int keyLength = 0;
  if(key != NULL) {
    keyLength = strlen(key);
  }

  if(keyLength > MAX_KEY_LENGTH) {
    printf("The key is too long!\nkey: %s\nlength: %d\n", key, keyLength);
    exit(-1);
  }

  int valueLength = 0;
  if(value != NULL) {
    valueLength = strlen(value);
  } 
  if(valueLength > MAX_VALUE_LENGTH) {
    printf("The value is too long!\nvalue: %s\nlength: %d\n", value, valueLength);
  }
  request->request_type = type;

  struct request_header* request_header = &(request->header);
  request_header->magic = MAGIC_REQUEST;
  request_header->data_type = 0;

  request_header->reserved[0] = 0;
  request_header->reserved[1] = 0;

  memset(&request_header->CAS, 0, 8);
  // We are using the opaque field for UIDs that will be sent back to use
  // in the response packet
  request_header->opaque = generateUID(worker);

  switch(requestType) {

    case STAT:{

      request_header->opcode = OP_STAT;

      break;

    }//End case STAT
    case ADD:{

      int body_length = 0;

      request_header->opcode = OP_ADD;

      request_header->key_length[0] = ((unsigned int)(strlen(key) & 0xff00))>>8;
      request_header->key_length[1] = (strlen(key) & 0xff);

      request->key = key;
      request->key_size = keyLength;
      request->value = value;
      request->value_size = valueLength;

      //Extra information
      request_header->extras_length = (char)8;
      request->extras = malloc(8);
      request->extras[0] = 0xde;
      request->extras[1] = 0xad;
      request->extras[2] = 0xbe;
      request->extras[3] = 0xef;
      request->extras[4] = 0;
      request->extras[5] = 0;
      request->extras[6] = 0;
      request->extras[7] = 0;

      body_length = 8 + keyLength + valueLength;
      #if DEBUG
      printf("body_length %d\n", body_length);
      #endif

      request_header->total_body_length[3] = (body_length & 0xff);
      request_header->total_body_length[2] = ((unsigned int)(body_length & 0xff00))>>8;
      request_header->total_body_length[1] = ((unsigned int)(body_length & 0xff0000))>>16;
      request_header->total_body_length[0] = ((unsigned int)(body_length & 0xff000000))>>24;
      break;

    }//End case REP
    case REP:{

      int body_length = 0;

      request_header->opcode = OP_REP;

      request_header->key_length[0] = ((unsigned int)(strlen(key) & 0xff00))>>8;
      request_header->key_length[1] = (strlen(key) & 0xff);

      request->key = key;
      request->key_size = keyLength;
      request->value = value;
      request->value_size = valueLength;

      //Extra information
      request_header->extras_length = (char)8;
      request->extras = malloc(8);
      request->extras[0] = 0xde;
      request->extras[1] = 0xad;
      request->extras[2] = 0xbe;
      request->extras[3] = 0xef;
      request->extras[4] = 0;
      request->extras[5] = 0;
      request->extras[6] = 0;
      request->extras[7] = 0;

      body_length = 8 + keyLength + valueLength;
      #if DEBUG
      printf("body_length %d\n", body_length);
      #endif

      request_header->total_body_length[3] = (body_length & 0xff);
      request_header->total_body_length[2] = ((unsigned int)(body_length & 0xff00))>>8;
      request_header->total_body_length[1] = ((unsigned int)(body_length & 0xff0000))>>16;
      request_header->total_body_length[0] = ((unsigned int)(body_length & 0xff000000))>>24;
      break;

    }//End case REP
    case DEL:{

      request_header->opcode = OP_DEL;

      request_header->key_length[0] = ((unsigned int)(strlen(key) & 0xff00))>>8;
      request_header->key_length[1] = (strlen(key) & 0xff);

      request->key = key;
      request->key_size = keyLength;

      request_header->extras_length = 0;
      request->extras = NULL;

      request->value = NULL;
      request->value_size = 0;
      request->extras = NULL;

      int body_length = keyLength;

      request_header->total_body_length[3] = (body_length & 0xff);
      request_header->total_body_length[2] = ((unsigned int)body_length & 0xff00)>>8;
      request_header->total_body_length[1] = ((unsigned int)body_length & 0xff0000)>>16;
      request_header->total_body_length[0] = ((unsigned int)body_length & 0xff000000)>>24;

      break;

    }//End case DEL
    case INCR:{

      request_header->opcode = OP_INCR;

      request_header->key_length[0] = ((unsigned int)(strlen(key) & 0xff00))>>8;
      request_header->key_length[1] = (strlen(key) & 0xff);

      request->key = key;
      request->key_size = keyLength;

      //Extra information
      // 8 byte value to add / subtract
      // 8 byte initial value (unsigned)
      // 4 byte expiration time

      request_header->extras_length = (char)20;
      request->extras = malloc(20);
      //request->extras[12] = 1;//value++
      //request->extras[3] = 0xff;
      //request->extras[2] = 0xff;
      //request->extras[1] = 0xff;
      //request->extras[0] = 0xff;
      request->extras[7] = 1;//value++
      request->extras[16] = 0xff;
      request->extras[17] = 0xff;
      request->extras[18] = 0xff;
      request->extras[19] = 0xff;
      //Right now, the expiration is 0, so we'll have to follow up the incr with a set if it fails.

      request->value = NULL;
      request->value_size = 0;

      int body_length = keyLength + 20;

      request_header->total_body_length[3] = (body_length & 0xff);
      request_header->total_body_length[2] = ((unsigned int)body_length & 0xff00)>>8;
      request_header->total_body_length[1] = ((unsigned int)body_length & 0xff0000)>>16;
      request_header->total_body_length[0] = ((unsigned int)body_length & 0xff000000)>>24;

      break;

    }//End case INCR
    case SET:{

      int body_length = 0;

      request_header->opcode = OP_SET;

      request_header->key_length[0] = ((unsigned int)(strlen(key) & 0xff00))>>8;
      request_header->key_length[1] = (strlen(key) & 0xff);

      request->key = key;
      request->key_size = keyLength;
      request->value = value;
      request->value_size = valueLength;

      //Extra information
      request_header->extras_length = (char)8;
      request->extras = malloc(8);
      request->extras[0] = 0xde;
      request->extras[1] = 0xad;
      request->extras[2] = 0xbe;
      request->extras[3] = 0xef;
      request->extras[4] = 0;
      request->extras[5] = 0;
      request->extras[6] = 0;
      request->extras[7] = 0;

      body_length = 8 + keyLength + valueLength;
      #if DEBUG
      printf("body_length %d\n", body_length);
      #endif

      request_header->total_body_length[3] = (body_length & 0xff);
      request_header->total_body_length[2] = ((unsigned int)(body_length & 0xff00))>>8;
      request_header->total_body_length[1] = ((unsigned int)(body_length & 0xff0000))>>16;
      request_header->total_body_length[0] = ((unsigned int)(body_length & 0xff000000))>>24;
      break;
    }
  case GET:{

      int body_length = 0;

      request_header->opcode = OP_GET;
      request_header->key_length[0] = ((unsigned int)(keyLength & 0xff00))>>8;
      request_header->key_length[1] = (keyLength & 0xff);

      request->key = key;
      request->key_size = keyLength;
      request->header.extras_length = (char)0;

      request->value = NULL;
      request->value_size = 0;
      request->extras = NULL;

      body_length = keyLength;

      request_header->total_body_length[3] = (body_length & 0xff);
      request_header->total_body_length[2] = ((unsigned int)(body_length & 0xff00))>>8;
      request_header->total_body_length[1] = ((unsigned int)(body_length & 0xff0000))>>16;
      request_header->total_body_length[0] = ((unsigned int)(body_length & 0xff000000))>>24;
      //printf("In get\n");
      //printf("body_length is %d\n", body_length);
      //int i;
      //for(i = 0; i < 4; i++){
      //  printf("%8x ", request_header->total_body_length[i]);
      //}
      //printf("\n");


      break;
    }
  case GETQ:{

      int body_length = 0;

      request_header->opcode = OP_GETQ;
      request_header->key_length[0] = ((unsigned int)(keyLength & 0xff00))>>8;
      request_header->key_length[1] = (keyLength & 0xff);

      request->key = key;
      request->key_size = keyLength;
      request->header.extras_length = (char)0;

      request->value = NULL;
      request->value_size = 0;
      request->extras = NULL;

      body_length = keyLength;

      request_header->total_body_length[3] = (body_length & 0xff);
      request_header->total_body_length[2] = ((unsigned int)body_length & 0xff00)>>8;
      request_header->total_body_length[1] = ((unsigned int)body_length & 0xff0000)>>16;
      request_header->total_body_length[0] = ((unsigned int)body_length & 0xff000000)>>24;

      break;
    }

  }//End switch

  return request;

}//End generateRequest()

