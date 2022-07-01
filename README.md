# Code Hiearchy
common -> crypto -> secure_comm -> c_api  -> test
                    load_config -----��

# writing function rules

void function(return_pointer, input ...)

every return and input buffers and lengths input with pointers

void function(unsigned char * ret, unsigned int * ret_length, unsigned char * input_buf, unsigned int * input_buf_length)

# C API

**void load_config()**

- ?���? ?��?��?�� input?���? ?��?���? ?��?��?�� sender, purpose, number of keys, crypto spec, pubkey path, privkey path ?��?�� ?��?��?�� config ?��?���? 불러?��?�� ?��?��
- config ?��?��??? user�? ?��?��?�� ?�� ?���? ?��공할 ?��?��
- ?���? ?��?��?��?�� load ?��게되�? high computation, long running time?�� 발생?���?�? ?���? ?��?���? 만듦
- return struct config

**void get_session_key()**
- entity client�? session key�? ?��?�� 과정
- input?��로는 struct config
- return struct session_key

**void secure_connection()**
- entity server?���? secure connection?�� ?��기위?�� 과정
- input?��로는 port, IP address, session key�? ?��?��
- return secure socket

**void send_secure_message() **
- send secure message by encrypting with session key
- input?��로는 session key, secure socket, message�? ?��?��

**void wait_connection_message()**
- entity server�? client?�� ?��?��?�� 기다리는 과정
- input?��로는 struct config
- return struct session_key

#compile

$make
$./test
