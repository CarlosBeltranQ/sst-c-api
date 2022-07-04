#include "c_api.h"

int main()
{
    // char path[] = "a.config";
    // config * config_info = load_config(path);

    // int serv_sock = init_server(config_info);

    const char * PORT_NUM = "21100";

    int BUF_SIZE = 1024;
    unsigned char buf[1024];

    int serv_sock, clnt_sock;
    struct sockaddr_in serv_adr, clnt_adr;
    struct timeval timeout;
    fd_set reads, cpy_reads;
    
    socklen_t adr_sz;
    int fd_max, str_len, fd_num, i;

    serv_sock=socket(PF_INET, SOCK_STREAM, 0);
    if(serv_sock == -1){
        error_handling("socket() error");
    }
    memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_adr.sin_port=htons(atoi(PORT_NUM));
    
    if(bind(serv_sock, (struct sockaddr*) &serv_adr, sizeof(serv_adr))==-1) {
        printf("bind() error");
    }
    if(listen(serv_sock, 5)==-1) {
        printf("listen() error");
    }
    
    FD_ZERO(&reads);		// fd_set �ʱ�ȭ
    FD_SET(serv_sock, &reads);	// ���� ������ ���� ������� ����
    fd_max=serv_sock;           // �ִ� ���� ��ũ���� ��

    while(1)
    {
        cpy_reads=reads;			// ���� fd_set ����
        timeout.tv_sec=5;
        timeout.tv_usec=5000;		// Ÿ�Ӿƿ� ����
        
        if((fd_num=select(fd_max+1, &cpy_reads, 0, 0, &timeout))==-1) {
            break;
        } // ���� ���� ���ϸ� �����Ƿ� connect ���� ��û �� �������Ͽ� �����Ͱ� ������ ��
        
        if(fd_num==0) {
            continue;
        } // Ÿ�� �ƿ� �� continue
	for(i=0; i<fd_max+1; i++)
        {
            if(FD_ISSET(i, &cpy_reads))
            {
                if(i==serv_sock)     
                {
                    adr_sz=sizeof(clnt_adr);
                    clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_adr, &adr_sz);
                    FD_SET(clnt_sock, &reads);
                    if(fd_max<clnt_sock)
                        fd_max=clnt_sock;
                    printf("connected client: %d \n", clnt_sock);
                    
                } // ��ȭ�� �Ͼ ������ ���� �����̸� connect ��û�� ���
                else 
                {
                    str_len=read(i, buf, BUF_SIZE);//TODO: start auth connection from here. read_clnt_sock.
                    if(str_len==0)    // close request!
                    {
                        FD_CLR(i, &reads);
                        close(i);
                        printf("closed client: %d \n", i);
                    }
                    else
                    {
                        printf("Hi");
                        write(i, buf, str_len);    // echo!
                        printf("Hello");
                    }
                } // �ٸ� ������ ��쿡�� ������ read
            }
        }
    }
    close(serv_sock);
    return 0;
}
