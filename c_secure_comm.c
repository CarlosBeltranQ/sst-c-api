#include "c_secure_comm.h"

/*
function:   prints the seq_num & message.
            This decrypts the received 'payload', with the 'session_key', and calculates the seq_num.
input: seq_num(the seq_num must be saved), payload(data to decrypt), session_key
*/

//TODO: debugging �ʿ��Ҽ��� ����.
void receive_message(unsigned int * seq_num, unsigned char * payload, unsigned int payload_length, session_key * session_key)
{
    //TODO: check validity
    // ���������� ���� ������ָ� ������? �Լ��� �ϳ� ����?
    unsigned int decrypted_length;
    unsigned char decrypted = symmetric_decrypt_authenticate(payload, payload_length, session_key->mac_key, MAC_KEY_SIZE, session_key->cipher_key, CIPHER_KEY_SIZE, AES_CBC_128_IV_SIZE, &decrypted_length);
    *seq_num = read_unsigned_int_BE(decrypted, SEQ_NUM_SIZE);
    printf("Received seq_num: %d\n", *seq_num);
    printf("%s\n", decrypted+SEQ_NUM_SIZE);
}

//TODO: ���������� �ϸ� ������.
void check_validity(){}