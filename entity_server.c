#include "c_api.h"

int main()
{
    // char path[] = "a.config";
    // config * config_info = load_config(path);

    // int serv_sock = init_server(config_info);

    int serv_sock, clnt_sock;
    const char * PORT_NUM = "21100";

    struct sockaddr_in serv_addr, clnt_addr;
    socklen_t clnt_addr_size;
    serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    if(serv_sock == -1){
        error_handling("socket() error");
    }
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port=htons(atoi(PORT_NUM));

    if(bind(serv_sock, (struct sockaddr*) &serv_addr, sizeof(serv_addr))==-1){
        error_handling("bind() error");
    }

    if(listen(serv_sock, 5)==-1){
        error_handling("listen() error");
    }
    clnt_addr_size = sizeof(clnt_addr);
    clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr, &clnt_addr_size);
    if(clnt_sock==-1){
        error_handling("accept() error");
    }

    char path[] = "server.config";
    config_t * config = load_config(path);
    session_key_t * s_key = server_secure_comm_setup(config, clnt_sock);

    printf("finished\n");
    pthread_t thread;
    arg_struct_t args = {
        .sock = clnt_sock,
        .s_key = s_key
    };
    pthread_create(&thread, NULL, &receive_thread, (void *)&args);
    sleep(1);

    send_secure_message("Hello World", strlen("Hello World"), s_key, clnt_sock);
    sleep(1);
    send_secure_message("Hello Dongha", strlen("Hello Dongha"), s_key, clnt_sock);
    sleep(1);
    // send_secure_message("Hello Yeongbin", strlen("Hello Yeongbin"),&session_key_list[0], sock);
    // sleep(10);
    // send_secure_message("Hello Yoonsang", strlen("Hello Yoonsang"),&session_key_list[0], sock);

    sleep(60);
}










/*
//Multiplexing version.
main()
{
    config = load_config();
    init_server(&config);

    int serv_sock;
    int clnt_sock = accept(serv_sock);
    data_length = read();
    session_key_t s_key = server_secure_comm_setup(data, data_length, clnt_sock, )
    parse -> 

    pthread_create(&wait_thread, NULL, &wait_connection, (void *)&args);
    //this covers all connections and receiving messages.

    send_to_everyone(); 
}
*/

/*
2. wait_server ù server socket ���ÿ���

    2-1 
            const char * PORT_NUM = "21100";
            struct sockaddr_in serv_addr;
            int serv_sock = socket(PF_INET, SOCK_STREAM, 0);
            if(serv_sock == -1)
                error_handling("socket() error");
            memset(&serv_addr, 0, sizeof(serv_addr));
            serv_addr.sin_family = AF_INET;
            serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
            serv_addr.sin_port=htons(atoi(PORT_NUM));

            if(bind(serv_sock, (struct sockaddr*) &serv_addr, sizeof(serv_addr))==-1)
                error_handling("bind() error");
            if(listen(serv_sock, 5)==-1)
                error_handling("listen() error");
            return serv_sock;

    �̷� �͵��� user �� ���� �Է��ؾ� �ϴ°��� �ƴϸ�,
    2-2
            int serv_sock = init_server(config *config_info); 
            �̷������� config�� ���� �����;��ϴ°���?
 2-2��
        

3. 2���� ���� ������ ���� serv_sock�� �̿��Ұǵ�,

    int client_sock;
    session_key_t s_key;
    accept()
    server_waits_client(serv_sock, client_sock, &s_key);
    // �� �κ��� accept���� blocking ��.
    print_recevied_message(client_sock, &s_key);
    �������θ� ����� �ǰ� thread�� ����� client���� ���� user�� �ؾ��ϴ� ���ΰ�?

    accept������ user��. 


4. send �� ��ü���� ���� + ���� client���� �������� ������ �ϴ°�? �ƴϸ� ��ü���� ������ user�� �˾Ƽ� �ؾ��ϴ°��ΰ�?
    ���� ��ü ������ ������ �Ѵٸ�, client ������ api���� �ؾ����� �ʳ�?

    ���� client���� ����

5. server_state �� ������ �Ѱܾ��ϴµ� �̰� ��� �ؾ��ϳ�?
    server_state��, 
    #define IDLE 0 //start state.
    #define HANDSHAKE_1_SENT 10
    #define HANDSHAKE_1_RECEIVED 21
    #define HANDSHAKE_2_SENT 22
    #define IN_COMM 30
   �� �ִµ�, �������� �ش� client_sock�� IN_COMM state�� �ִ°��� Ȯ���ؾ��ϴµ�,
   api�� server_waits_client() �� print_recevied_message() �� ������ server_state�� ��ǲ���� �Ѱܾ� �ϴµ�, ��� �ѱ�°� �������?
    �� server_state�� client_sock ���� ���� �ʿ�. �� �κе� user�� �ؾ��� ���ΰ���?

���� ���� �� ��� ����ڰ�. client, thread ������ �� ����ڰ�.

.h���Ͽ� comment ���ֱ�.

high level�� .h�� 

usage���� �� ��ǲ���� ��Ծ��̴��� ����. ex) �� ���� ������� ���� �����ͻ������~~�̷���.

*/