#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <string.h>

/* some important info
size of char - 1 byte (8 bits)
int - 4 bytes
uint16_t - 2 bytes (16 bits)
function sizeof (sth) returns number of BYTES in format long unsigned int (lu)
*/

//czy fwrite nadpisuje czy dopisuje
//po co sa te dziwne wskazniki kolo strutkur
//dlaczwgo w 293 dziala calloc a statycznie wywala warning
//czy w 120 musze pisac ten int w nawiasie bo bez tez dziala

typedef uint16_t WORD;
typedef uint32_t DWORD;
typedef int32_t LONG;

// https://docs.microsoft.com/en-us/windows/win32/api/wingdi/ns-wingdi-bitmapfileheader
typedef struct tagBITMAPFILEHEADER {
    WORD  bfType;
    DWORD bfSize;
    WORD  bfReserved1; //if bmp file is encoded, then this byte is equal to 1;
    WORD  bfReserved2;
    DWORD bfOffBits;
} BITMAPFILEHEADER;

// https://docs.microsoft.com/pl-pl/previous-versions/dd183376(v=vs.85)
typedef struct tagBITMAPINFOHEADER {
    DWORD biSize;
    LONG  biWidth;
    LONG  biHeight;
    WORD  biPlanes;
    WORD  biBitCount;
    DWORD biCompression;
    DWORD biSizeImage;
    LONG  biXPelsPerMeter;
    LONG  biYPelsPerMeter;
    DWORD biClrUsed;
    DWORD biClrImportant;
    WORD  how_many_padded;
} BITMAPINFOHEADER;

void read_bottom_up (FILE * source, BITMAPFILEHEADER * file_header, BITMAPINFOHEADER * info_header, unsigned char **pixels){

    fread(&file_header->bfType, 2, 1, source); //where has to be stored, how many per one, how may times, from where
    fread(&file_header->bfSize, 4, 1, source);
    fread(&file_header->bfReserved1, 2, 1, source);
    fread(&file_header->bfReserved2, 2, 1, source);
    fread(&file_header->bfOffBits, 4, 1, source);

    //reading part with info header
    fread(&info_header->biSize, 4, 1, source);
    fread(&info_header->biWidth, 4, 1, source);
    fread(&info_header->biHeight, 4, 1, source);
    fread(&info_header->biPlanes, 2, 1, source);
    fread(&info_header->biBitCount, 2, 1, source);
    fread(&info_header->biCompression, 4, 1, source);
    fread(&info_header->biSizeImage, 4, 1, source);
    fread(&info_header->biXPelsPerMeter, 4, 1, source);
    fread(&info_header->biYPelsPerMeter, 4, 1, source);
    fread(&info_header->biClrUsed, 4, 1, source);
    fread(&info_header->biClrImportant, 4, 1, source);

    //reading pixels_array
    int row_size_in_bits = info_header->biBitCount * info_header->biWidth;
    int row_size = row_size_in_bits/8;
    int row_size_padding = (row_size_in_bits + 31)/32*4;
    int how_many_padded = row_size_padding - row_size;
    info_header->how_many_padded = how_many_padded;
    int all_pixels = row_size * info_header->biHeight;
    //printf("row size %d\nrow size with padding %d\nhow many padded %d\nall pixels %d\n", row_size, row_size_padding, how_many_padded, all_pixels);


    *pixels = malloc (sizeof(char) * all_pixels);
    fseek(source, file_header->bfOffBits, 0);
    unsigned char *current = *pixels;

    for (int i=0;i<info_header->biHeight;i++){
        //printf("%d ", i);
        fread(current, sizeof(char), row_size, source);
        fseek (source, how_many_padded, 1);
        current+= row_size;

    }
}


void print_file_header (BITMAPFILEHEADER * temp){
    printf("bfType:          %X\n", temp->bfType);
    printf("bfSize:          %d\n", temp->bfSize);
    printf("bfReserved1:     %X\n", temp->bfReserved1);
    printf("bfReserved2:     %X\n", temp->bfReserved2);
    printf("bfOffBits:       %d\n", temp->bfOffBits);
}

void print_info_header (BITMAPINFOHEADER *temp){
    printf("biSize:          %d\n", temp->biSize);
    printf("biWidth:         %d\n", temp->biWidth);
    printf("biHeight:        %d\n", temp->biHeight);
    printf("biPlanes:        %d\n", temp->biPlanes);
    printf("biBitCount:      %d\n", temp->biBitCount);
    printf("biCompression:   %d\n", temp->biCompression);
    printf("biSizeImage:     %d\n", temp->biSizeImage);
    printf("biXPelsPerMeter: %d\n", temp->biXPelsPerMeter);
    printf("biYPelsPerMeter: %d\n", temp->biYPelsPerMeter);
    printf("biClrUsed:       %d\n", temp->biClrUsed);
    printf("biClrImportant:  %d\n", temp->biClrImportant);

}



int interval(unsigned char temp){

    return (int)temp/16;
}


void histogram(LONG szerokosc, WORD ilosc_bitow, DWORD kompresja, LONG wysokosc, unsigned char *tablica){
    if (kompresja != 0 || ilosc_bitow != 24) {
        printf("Histogram is supported only for uncompressed 24-Bit maps.");
        return;
    }
    long num_of_pix = szerokosc * wysokosc;
    //double temp = floor((ilosc_bitow * szerokosc + 31)/32)*4;
    int *hist_r = calloc(16, sizeof(int));
    int *hist_g = calloc(16, sizeof(int));
    int *hist_b = calloc(16, sizeof(int));
    //nie wiem jaka kolejnosc kolorow zalozyc, ale powiedzmy ze tak jak jest zakodowane w pliku
    //Blue Green Red
    for (long indx = 0; indx < num_of_pix*3 ; ++indx) {
        if (indx % 3 == 0) {
            hist_b[interval(tablica[indx])]++;
        }
        else if(indx  % 3 == 1){
            hist_g[interval(tablica[indx])]++;
        }
        else{
            hist_r[interval(tablica[indx])]++;
        }
    }
    int temp1 = 0;
    int temp2 = 15;
    printf("Histogram for red:\n");
    for (int i = 0; i < 16; ++i) {
        printf("%d - %d: %.2f%%\n", temp1, temp2, ((double) hist_r[i]/num_of_pix) * 100);
        temp1 += 16;
        temp2 += 16;
    }
    temp1 = 0;
    temp2 = 15;
    printf("Histogram for green:\n");
    for (int i = 0; i < 16; ++i) {
        printf("%d - %d: %.2f%%\n", temp1, temp2, ((double) hist_g[i]/num_of_pix) * 100);
        temp1 += 16;
        temp2 += 16;
    }
    temp1 = 0;
    temp2 = 15;
    printf("Histogram for blue:\n");
    for (int i = 0; i < 16; ++i) {
        printf("%d - %d: %.2f%%\n", temp1, temp2, ((double) hist_b[i]/num_of_pix) * 100);
        temp1 += 16;
        temp2 += 16;
    }
    free(hist_b);
    free(hist_r);
    free(hist_g);
}
void writing_grey (BITMAPFILEHEADER *file_header, BITMAPINFOHEADER *info_header, unsigned char *pixels, FILE *source, char *fileName){
    if (info_header->biCompression != 0 || info_header->biBitCount != 24){
        printf("converting to grey scale is supported only for uncompressed 24-Bit maps.");
        return;
    }
    fseek (source, 0, 0);

    FILE * picture_grey = fopen( fileName, "w");
    unsigned char bufor[file_header-> bfOffBits];
    fread (bufor, 1,file_header->bfOffBits, source);
    fwrite(bufor, 1,file_header->bfOffBits, picture_grey);

    int temp;
    char padding = 0;
    for (int i=0;i<info_header->biHeight;i++){
        for (int j=0;j<info_header->biWidth;j++){
            temp = 0;
            //printf("%d ", i* info_header->biWidth*3 + j*3);
            temp+=pixels[i* info_header->biWidth*3 + j*3];
            temp+=pixels[i* info_header->biWidth*3 + j*3+1];
            temp+=pixels[i* info_header->biWidth*3 + j*3+2];
            temp/=3;
            //printf("%X ", grey);
            for (int k=0;k<3;k++){
                fwrite(&temp, 1, 1, picture_grey);
            }
        }
        for(int j=0;j<info_header->how_many_padded;j++)
            fwrite (&padding, 1, 1, picture_grey);
    }

    fclose(picture_grey);
}

void writing_steganography (BITMAPFILEHEADER *file_header, BITMAPINFOHEADER *info_header, unsigned char *pixels, FILE *source, char *fileName){
    if (info_header->biCompression != 0 || info_header->biBitCount != 24){
        printf("steganography is supported only for uncompressed 24-Bit maps.");
        return;
    }
    fseek (source, 0, 0);

    FILE * picture_steg = fopen(fileName, "w");
    unsigned char bufor[file_header-> bfOffBits];
    fread (bufor, 1,6 , source);
    fwrite(bufor, 1,6, picture_steg);
    char temp = 1;
    fwrite (&temp, 1, 1, picture_steg);
    int how_many_to_end =file_header->bfOffBits - 7;
    fseek(source, 1, 1);
    fread (bufor, 1, how_many_to_end, source);
    fwrite(bufor, 1, how_many_to_end, picture_steg);

    char padding = 0;
    for (int i=0;i<info_header->biHeight;i++){
        for (int j=0;j<info_header->biWidth*3;j++){
            fwrite( &pixels[i*info_header->biWidth*3 + j], 1, 1, picture_steg);
        }
        for(int j=0;j<info_header->how_many_padded;j++)
            fwrite (&padding, 1, 1, picture_steg);
    }

    fclose(picture_steg);
}
void encoding (unsigned char *array, char* text_to_hide){
    int len = (int) strlen(text_to_hide);
    int how_many_bits_needed = (len+1)*8;
    //first I want to set all needed the least significant bits of byte to 0
    for (int i=0;i<how_many_bits_needed;i++){
        if (array[i] % 2 == 1)
            array[i]--;
    }
    char *bin_str = calloc(how_many_bits_needed, sizeof(char));
    int counter = 0;
    int temp = len;
    while (temp != 0){
        if (temp % 2 == 1)
            bin_str[counter] = 1;
        temp /= 2;
        counter++;
    }

    for (int i = 0; i < len +1; ++i) {
        temp = (int) text_to_hide[i];
        counter = 0;
        while (temp != 0) {
            if (temp % 2 == 1)
                bin_str[counter + (i + 1) * 8] = 1;
            temp /= 2;
            counter++;
        }
    }
    /*for (int i = 0; i < len + 1; ++i) {
        for (int j = 0; j < 8; ++j) {
            printf("%d  ", bin_str[i*8 + j]);
        }
        printf("\n");
    }*/
    for (int i = 0; i < how_many_bits_needed; ++i) {
        if (bin_str[i] == 1){
            array[i]++;
        }
    }
    free(bin_str);
    printf("Stenography encoded.\n");
}


int bin_to_dec (char * bin){
    int dec = 0, base = 1;
    for (int i=0;i<8;i++){
        if (bin[i] == 1)
            dec+= base;
        base*=2;
    }
    return dec;
}

void decoding (unsigned char *array, char **text){
    char* bufor = calloc(8,1);
    for (int i=0;i<8;i++)
        if (array[i] % 2 == 1)
            bufor[i] = 1;

    int len = bin_to_dec(bufor);
    *text = malloc((len+1) * sizeof(char));
    int i;
    for (i=0;i<len;i++){
        for (int j=0;j<8;j++){
            if (array[(i+1) * 8 + j ] % 2 == 1)
                bufor[j] = 1;
            else
                bufor[j] = 0;
        }
        (*text)[i] = bin_to_dec(bufor);
    }
    (*text)[i] = '\0';
    free(bufor);
}





int main (int argc, char *argv[]){
    BITMAPFILEHEADER file_header;
    BITMAPINFOHEADER info_header;
    unsigned char *pixels_array;
    FILE *picture;

    if (argc == 3){

        picture = fopen (argv[1], "rb");
        if ( !picture ){
            printf("error opening file: %s\n", argv[1]);
            return -1; }

        read_bottom_up (picture, &file_header, &info_header, &pixels_array);
        print_file_header (&file_header);
        print_info_header(&info_header);
        histogram(info_header.biWidth, info_header.biBitCount, info_header.biCompression, info_header.biHeight, pixels_array);
        writing_grey(&file_header, &info_header, pixels_array, picture,argv[2]);
        fclose(picture);
        free(pixels_array);

    }

    else if (argc == 4){
        picture = fopen(argv[1], "rb");
        read_bottom_up (picture, &file_header, &info_header, &pixels_array);
        if ( (int) strlen(argv[3])+1 > info_header.biWidth * info_header.biHeight * 3 || strlen(argv[3]) > 255){
            printf("error: text is too long\n");
            return -3;
        }
        encoding (pixels_array, argv[3]);
        writing_steganography(&file_header, &info_header, pixels_array, picture,argv[2]);
        fclose(picture);
        free(pixels_array);


    }
    else if (argc == 2){
        picture = fopen (argv[1], "rb");
        if ( !picture ){
            printf("error opening file: %s\n", argv[1]);
            return -1; }
        read_bottom_up(picture, &file_header, &info_header, &pixels_array);
        print_file_header(&file_header);
        print_info_header(&info_header);
        if (file_header.bfReserved1 != 1){
            fclose(picture);
            free(pixels_array);
            return 0;
        }
        printf("Decode steganography? [Y/n]\n");
        char decision;
        scanf ("%c", &decision);
        if (decision == 'y' || decision == 'Y'){
            char *hidden_text;
            decoding (pixels_array, &hidden_text);
            printf("Hidden message: %s\n", hidden_text);
            free(hidden_text);

        }
        else if (decision != 'n')
            printf("You wrote wrong letter  so the decision is no.\n");
        free(pixels_array);
        fclose(picture);
    }
    else{
        printf("wrong input\n");
        return -2;
    }

    return 0;
}