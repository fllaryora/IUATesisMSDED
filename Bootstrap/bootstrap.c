#include <stdio.h>
#include <mongo.h>

#ifndef FALSE
#define FALSE (0)
#endif
#ifndef TRUE
#define TRUE (!FALSE)
#endif

const char* baseName = "botqueuedb.projects";
const char *db = "botqueuedb";
const char *coll = "projects";
int isThereProjectRunning( mongo *conn);

void tutorial_empty_query( mongo *conn);

int main() {
  mongo conn[1];
  int status = mongo_client( conn, "127.0.0.1", 27017 );

  if( status != MONGO_OK ) {
      switch ( conn->err ) {
        case MONGO_CONN_NO_SOCKET:  printf( "no socket\n" ); return 1;
        case MONGO_CONN_FAIL:       printf( "connection failed\n" ); return 1;
        case MONGO_CONN_NOT_MASTER: printf( "not master\n" ); return 1;
        default: break;
      }
  }
  printf( "connect!!\n" );
  /* hay algun proyecto corriendo o que no se recolectaron los datos*/
  if (isThereProjectRunning(conn)) {
    printf( "a recolectar datos!!\n" );
  } else{
    printf( "a lanzar proyectos pendientes!!\n" );
  }
  //tutorial_empty_query(conn);
  mongo_destroy( conn );

  return 0;
}

int isThereProjectRunning( mongo *conn){
  bson query;
  /* query:  { "state":"E", "finishingStamp":null } */
  bson_init( &query );
  {
    bson_append_string( &query, "state", "E" );
    bson_append_null( &query, "finishingStamp");
    bson_append_finish_object( &query );
  }
  bson_finish( &query );
  if (mongo_count(conn, db, coll, &query ) != 0 ) return TRUE;
  else return FALSE;
}

void tutorial_empty_query( mongo *conn) {
  mongo_cursor cursor[1];
  mongo_cursor_init( cursor, conn, baseName );

  while( mongo_cursor_next( cursor ) == MONGO_OK )
    bson_print( &cursor->current );

  mongo_cursor_destroy( cursor );
}