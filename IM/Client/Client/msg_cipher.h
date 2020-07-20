#pragma once
#ifndef MSG_CIPHER_H
#define MSG_CIPHER_H

#define CIPHER_KEY 3

void msg_encipher(char *msg, int key);
void msg_decipher(char *msg, int key);

#endif // !MSG_CIPHER_H

