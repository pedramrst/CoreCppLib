#include "Constants.h"

namespace Constants
{
    
    const char *DEFAULT_DB_CONNECTION_STR = "dbname=db user=management_user password=management_user host=localhost port=5432";

    const char *DEFAULT_KAFKA_BROKER = "localhost:9092";

    const char *DEFAULT_RAW_TOPIC = "raw";
    const char *DEFAULT_PREPROCESSOR_TOPIC = "preprocessed";
    const char *DEFAULT_PREPROCESSOR_GROUP_ID = "preprocessor_group";

    const char *DEFAULT_REDIS_HOST = "localhost";
    const int DEFAULT_REDIS_PORT = 6379;
}
