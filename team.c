#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

#define xsize 570
#define ysize 456
#define input_filename "character.bmp"
#define output_filename "test_image.bmp"

int is_success_open_file(FILE* filename);       // 파일을 성공적으로 열었는지 확인하는 함수
//void is_BMP(BITMAPFILEHEADER hf);               // open한 파일의 형식이 BMP가 맞는지 확인하는 함수
void read_image(BITMAPFILEHEADER* hf, BITMAPINFOHEADER* hInfo, RGBQUAD hRGB[256], unsigned char InputImg[xsize][ysize]);    // open한 이미지의 픽셀을 읽는 함수
void copy_image(unsigned char InputImg[xsize][ysize], unsigned char OutputImg[xsize][ysize]);       // read_image 함수에서 읽은 픽셀값을 쓰기 전에 저장하는 함수
void write_image(BITMAPFILEHEADER* hf, BITMAPINFOHEADER* hInfo, RGBQUAD hRGB[256], unsigned char OutputImg[xsize][ysize]);  // copy_image에서 저장한 픽셀을 통해 이미지 파일을 만드는 함수
void print_result(BITMAPFILEHEADER hf, BITMAPINFOHEADER hInfo, RGBQUAD hRGB[256]);

int main(void)
{
    // 밑의 값들은 Windows.h 헤더파일에서 값이 정해져있음
    BITMAPFILEHEADER hf;        // "파일정보헤드" 구조체 변수 선언
    BITMAPINFOHEADER hInfo;     // "이미지정보헤드" 구조체 변수 선언
    RGBQUAD hRGB[256];          // 팔레트 정보를 위한 배열 선언

    unsigned char InputImg[xsize][ysize];       // 입력 이미지
    unsigned char OutputImg[xsize][ysize];      // 출력 이미지


    // 이미지 파일 읽기 (Windows.h 헤더파일의 내장 함수 fread를 통해서)
    read_image(&hf, &hInfo, hRGB, InputImg);


    print_result(hf, hInfo, hRGB);

    // 읽은 이미지 파일의 픽셀들을 복사, OutputImg에 저장
    copy_image(InputImg, OutputImg);



    // 픽셀값이 저장된 OutputImg를 통해서 이미지 저장
    write_image(&hf, &hInfo, hRGB, OutputImg);



    return 0;
}

int is_success_open_file(FILE* filename)
{
    if (filename == NULL)
    {
        printf("failed to open file\n\n");
        return 1;

    }
    else
    {
        printf("successed to open file\n\n");
        return 0;
    }
}

void read_image(BITMAPFILEHEADER* hf, BITMAPINFOHEADER* hInfo, RGBQUAD hRGB[256], unsigned char InputImg[xsize][ysize])
{
    FILE* fp;         // 파일 포인터 선언

    // 이미지 파일 open (파일을 읽거나 쓰기 위한 선행 작업). 아래의 코드로 fp는 다룰 파일을 지칭게 됨. rb의 뜻 : "r" : 읽기, "b" : 이진수로
    fp = fopen(input_filename, "rb");

    // 이미지 파일 열기를 성공적으로 마쳤는지 판단
    if (is_success_open_file(fp) == 1)
    {
        printf("EXIT program");
    }

    /*
    fread 함수의 첫 번째 인자는 각 구조체의 포인터
    2번째는 byte크기로 각 구조체의 크기를 byte 단위로 반환
    3번째에 있는 숫자는 읽기의 반복 횟수, 4번째는 읽을 파일을 가리킴
    */
    fread(hf, sizeof(BITMAPFILEHEADER), 1, fp);             // hf : BITMAPFILEHEADER 구조체의 포인터
    fread(hInfo, sizeof(BITMAPINFOHEADER), 1, fp);          // hInfo : BITMAPINFOHEADER의 구조체의 포인터
    fread(hRGB, sizeof(RGBQUAD), 256, fp);                  // hRGB : RGBQUA 구조체의 포인터
    fread(InputImg, xsize * ysize, 1, fp);                  // InputImg는 main함수에서 선언됐고, 배열명은 포인터기 때문에 InputImg는 포인터

    // 작업을 마쳤으니 close
    fclose(fp);
}

void copy_image(unsigned char InputImg[xsize][ysize], unsigned char OutputImg[xsize][ysize])
{
    // 이미지의 픽셀을 2차원으로 분해, OutputImg에 저장
    for (int i = 0; i < xsize; i++)
    {
        for (int j = 0; j < ysize; j++)
        {
            OutputImg[i][j] = InputImg[i][j];
        }
    }
}

void write_image(BITMAPFILEHEADER* hf, BITMAPINFOHEADER* hInfo, RGBQUAD hRGB[256], unsigned char OutputImg[xsize][ysize])
{
    FILE* fp;           // 파일 포인터 선언
    // 이미지 파일을 저장하기 위해서 다시 open. "w" : 쓰기
    fp = fopen(output_filename, "wb");

    // read_image에서 설명한 바와 같다. 다만, 3번째 인자는 쓰는 횟수
    fwrite(hf, sizeof(BITMAPFILEHEADER), 1, fp);
    fwrite(hInfo, sizeof(BITMAPINFOHEADER), 1, fp);
    fwrite(hRGB, sizeof(RGBQUAD), 256, fp);
    fwrite(OutputImg, xsize * ysize, 1, fp);

    /*
    이 주석은 fwrite(OutputImg, xsize * ysize, 1, fp); 이 코드가 문제가 있을 시에 바꿀 코드.
    for(int i = 0; i < xsize; i++)
    {
        fwrite(OutputImg[i], ysize, 1, fp);
    }
    */

    // 작업을 마쳤으니 close
    fclose(fp);
}

void print_result(BITMAPFILEHEADER hf, BITMAPINFOHEADER hInfo, RGBQUAD hRGB[256])
{
    printf("size of BITMAPINFOHEADER = %d\n", hInfo.biSize);                 // BITMAPINFOHEADER 구조체의 크기
    printf("WIDTH OF IMG = %d\n", hInfo.biWidth);               // 이미지의 가로 크기
    printf("HEIGHT OF IMG = %d\n", hInfo.biHeight);             // 이미지의 세로 크기
    printf("BYTE SIZE OF IMG = %d\n", hf.bfSize);                       // byte 단위로 파일 전체 크기
    printf("IS IMG BMP? = %hx.  if result : 4d42, it means BMP\n", hf.bfType);                      // "파일이 BMP"가 맞는지
    printf("NUMBER OF USED COLORS = %d\n", hInfo.biClrUsed);           // 실제 사용 색상 수
    printf("BIT PER PIXEL = %d\n\n\n", hInfo.biBitCount);         // 픽셀당 비트 수 (컬러, 흑백 구별)
    printf("EXIT program");
}