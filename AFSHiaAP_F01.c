#include <stdio.h>
#include <string.h>
#include <unistd.h>

char string[95];
int string_len;
char output_encrypt[256];
char output_decrypt[256];

int mod(int a, int b) {
    int r = a % b;
    return r < 0 ? r + b : r;
}

void do_encrypt(char input[], int key){
    int input_len=  strlen(input);

    for (int i=0; i < input_len; i++){
        if (input[i] == '/'){
            output_encrypt[i] = '/';
        }
        else {
            // get index in string
            char *e;
            e=  strchr(string, input[i]);
            int index = (int)(e - string);

            // encrypt
            int new_index=  mod((index + key), string_len);
            // printf("%d, %d, %d, %d\n", new_index, index, key, string_len);

            // output
            output_encrypt[i] = string[new_index];
        }
    }
}

void do_decrypt(char input[], int key){
    int input_len=  strlen(input);

    for (int i=0; i < input_len; i++){
        if (input[i] == '/'){
            output_decrypt[i] = '/';
        }
        else {
            // get index in string
            char *e;
            e=  strchr(string, input[i]);
            int index = (int)(e - string);

            // decrypt
            int old_index=  mod((index - key), string_len);
            // printf("%d, %d, %d, %d\n", old_index, index, key, string_len);

            // output
            output_decrypt[i] = string[old_index];
        }
    }
}

int main(){
    char input[256];
    int key=0;

    // index string 0 - 93, sisanya eof
    sprintf(string, "qE1~ YMUR2%c`hNIdPzi%c^t@(Ao:=CQ,nx4S[7mHFye#aT6+v)DfKL$r?bkOGB>}!9_wV']jcp5JZ&Xl|%c8s;g<{3.u*W-0", '"', '%', '\\');

    // cari panjang string, -2 (end of file (/0)) dan +1 karena mulai dari 0
    string_len= sizeof(string) - 1;

    // get input string
    printf("Masukan Input : \n");
    if (fgets(input, sizeof input, stdin) != NULL) {
      // menghitung panjang string
      size_t len = strlen(input);
      // menghapus newline
      if (len > 0 && input[len-1] == '\n') {
        input[--len] = '\0';
      }
    }

    // get input key
    printf("Masukan Key : \n");
    scanf("%d", &key);

    // do encrypt
    do_encrypt(input, key);
    printf("encrypt:\t%s\t%ld\n", output_encrypt, strlen(output_encrypt));

    // do decrypt
    do_decrypt(output_encrypt, key);
    printf("decrypt:\t%s\t%ld\n", output_decrypt, strlen(output_decrypt));

    // printf
    // printf("Encrypt: %s\nDecrypt: %s\n", output_encrypt, output_decrypt);

    // exit
    return 0;
}
