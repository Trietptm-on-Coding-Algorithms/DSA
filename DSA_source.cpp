﻿// DSA.cpp: определяет точку входа для консольного приложения.
//

#ifdef MSC_VER
#include "stdafx.h"
#endif
#include <stdio.h>
#include <iostream>
#include <string.h>
#include <locale>

using std::cout;
using std::endl;
using std::cin;

#define BYTE unsigned char
#define WORD unsigned short
#define DWORD unsigned long int
#define LEN1 11
#define LENBUF 130
#define max(a,b)  (((a)>(b))?(a):(b))
#define min(a,b)  (((a)<(b))?(a):(b))

//простые числа и ключи

WORD QQ[11]=
    {
     0xfe1b, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff,
     0xffff, 0xffff, 0x0000
     }; // модуль (для вычислений)
WORD XX[11]=
    {
     0xffff, 0x1111, 0x2222, 0x3333, 0x4444, 0x5555, 0x6666, 0x7777,
     0x8888, 0x9999, 0x0000
     }; //секретный ключ
WORD k[11]=
    {
     0xfdf3, 0xf2ff, 0xf4ff, 0xfff6, 0xf4ff, 0x664f, 0xffff, 0xffff,
     0xffff, 0xffff, 0x0001
     }; // разовый ключ, случайный эфемерный ключ
WORD Hnach[11]= 
    {
     0xfdf3, 0xf26f, 0x64ff, 0xfdf6, 0xf4fc, 0x624f, 0xffbb, 0x4fff,
     0xffff, 0xff66, 0x0005
     }; // начальное значение для вычисления G
WORD ODIN[11]=
    {
     0x0001, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	 0x0000, 0x0000, 0x0000 
     }; // длинная единица
WORD PP[11]=
    {
     0x3365, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff,
     0xffff, 0xffff, 0x006b
    }; //модуль Р=aQ+1,значение "а" должно быть таким, чтобы  Р было простым числом
DWORD ACONST[5] = { 0x67452301, 0xefcdab89, 0x98badcfe, 0x10325476, 0xc3d2e1f0 };


void hash_fun(DWORD *TTG2, DWORD *M2);
int hash_f(DWORD x, DWORD y, DWORD z, int i1);
void plus(int len, WORD * a, WORD * b, WORD * c);
void minus(int len, WORD * a, WORD * b, WORD * c);
void plus(int len, WORD * a, WORD * b, WORD * c);
void change_sign(int len, WORD * c);
int num_first_1(int len, WORD * a);
int Div(int len, WORD * a, WORD * b, WORD * c);
int divost(int len, WORD * a, WORD * b, WORD * c, WORD * r);
void mul2(int deg, int len, WORD * a, WORD * c);
int cmp(int len, WORD * a, WORD * b);
int obr(int len, WORD * a, WORD * N, WORD * c);
void umn(WORD *a,int n1 ,WORD *b,int n2,WORD *c,int n3);
void mod_p(WORD * a, WORD n1, WORD * p, WORD n2);
void step_mod(WORD * p, WORD * a, int n1, WORD * b, int n2, WORD * c);
void form_pod(int len1, WORD *g1, char *bb, WORD *skl, WORD *R1, WORD *S1);
void prov_pod(int len1, WORD *g1, char *bb, WORD *okl, WORD *R1, WORD *S1);



int main()
{
	
//setlocale (LC_CTYPE, "Russian_Russia.1251");
setlocale (LC_CTYPE, "");
cout << "***** DSA Signing Service*****"<<endl;
int i,len2=LEN1;
WORD GG[LEN1],Y[LEN1],R1[LEN1],S1[LEN1],X3[LEN1],X4[LEN1];
 char MP2[255];
cout<<"Введите сообщение, которое необходимо подписать"<<endl;
cin>>MP2;
// ="Student Kravtsov Alexey, gruppa 1101, facultet kompleksnaya bezopastnost'.";

cout<<"Генерация простых чисел и ключей";
getchar();
cout <<endl << "Простое число Р"<<endl;
cout<<"P = "<<endl;
for(i=0; i<len2; i++) printf("%04x ",PP[i]); 
cout <<endl;

cout <<endl << "160-ти битовое простое число Q"<<endl<<"Q = ";
for(i=0; i<len2; i++) printf("%04x ",QQ[i]); 
cout <<endl;

cout <<endl << "Случайное число Н"<<endl<<"H = ";
for(i=0; i<len2; i++) printf("%04x ",Hnach[i]); 
cout <<endl;

cout << "Случайное число Н, приведенное по модулю Р  "<<endl<<"H = ";
mod_p(Hnach,len2,PP,len2);
for(i=0; i<len2; i++) printf("%04x ",Hnach[i]); 
cout <<endl;

cout <<endl << "Секретный ключ х "<<endl; 
for(i=0; i<len2; i++) printf("%04x ",k[i]); 
cout <<endl;

cout << "Секретный ключ, приведенный по модулю Р"<<endl; 
mod_p(k,len2,QQ,len2);
for(i=0; i<len2; i++) printf("%04x ",k[i]); 
cout <<endl;

 // вычисление G=Hnach^[(P-1)/Q](mod P)
cout <<endl << "вычисление G=Hnach^[(P-1)/Q](mod P)"<<endl << "G = ";
minus(len2,PP,ODIN,X3);
Div(len2,X3,QQ,X4);
step_mod(PP,Hnach,len2,X4,len2,GG);

for(i=0; i<len2; i++) printf("%04x ",GG[i]); 

//проверка модуля Р: G^Q=1(mod P)
cout<<endl <<endl << "проверка модуля Р: G^Q=1(mod P)";
step_mod(PP,GG,len2,QQ,len2,X3);
printf("\nG^Q = "); 
for(i=0; i<len2; i++) printf("%04x ",X3[i]); 

//Вычисление открытого ключа по секретному ключу
step_mod(PP,GG,len2,XX,len2,Y);
cout <<endl<<endl << "Открытый ключ Y=G^x(mod P)"<<endl <<"Y = "; 
for(i=0; i<len2; i++) printf("%04x ",Y[i]); 
        

printf("\n\n\n ---Выработка подписи--- \n");
cout<<endl<<"Подписываемое сообщение"<<endl << MP2<<endl;

form_pod(len2, GG, MP2, k, R1, S1);


    /* Проверка подписи */
printf("\n\n ---Проверка подписи--- \n");

//Вывод открытого ключа
cout <<endl << "Открытый ключ"<<endl<<"Y = "; 
for(i=0; i<len2; i++) printf("%04x ",Y[i]); 
cout <<endl;

cout<<endl<<"Подписываемое сообщение"<<endl << MP2<<endl;
prov_pod(len2, GG, MP2, Y, R1, S1);

}
     /* Функция выполняет вычисление подписи */
void form_pod(int len, WORD *g1, char *bb, WORD *skl, WORD *R1, WORD *S1)
{

    int i,j,j1,z,z1,n,n1,n2,n3;
    BYTE MM[64];
    WORD M[LEN1],X5[2*LEN1],X6[LEN1],HH[LEN1];
    DWORD qq1,MM1[16],AAA1[5];

        /* 1)Вычисление хэш-функции подписываемых данных  */

        for(i=0; i<5; i++) AAA1[i]=ACONST[i];
        i=0;
		n=0;
		while (bb[i]!= '\0')
		{
			n++;
			i++;
		}

         cout<<endl<<"Сообщение, переведенное в коды ASCII "<<endl;
         j=0;
         for(i=0; i<n; i++) 
         {
          printf("%02x ",(BYTE)bb[i]);
          j++;  
          if(j==16) { j=0; printf("\n"); }
         }


		cout<<endl<<"Количество символов n = "<<n;
		n1=n/64;
		n2=n%64;
        cout<<endl<<"Количество блоков по 64 символа n1 = "<<n1;
		cout<<endl<<"Количество оставшихся символов n2 = "<<n2<<endl;
		
	   if(n2!=0) n3=n1+1;
       z1=0;

       for(z=0; z<n3; z++)
       {
        if(n1!=0) for(j1=0; j1<64; j1++) { MM[j1]= (BYTE)bb[z1]; z1++;}
            else {
                  for(j1=0; j1<n2; j1++) { MM[j1]=(BYTE)bb[z1]; z1++; }
                  for(j1=n2; j1<64; j1++) MM[j1]= 0x00;
                 }
  
        j=0;
        for(i=0; i<16; i++)
        { 
         qq1= (DWORD)MM[j+3]<<24 ^ (DWORD)MM[j+2]<<16 ^ (DWORD)MM[j+1]<<8; 
         MM1[i]=(DWORD)MM[j] ^qq1;
         j+=4;
        }   
           
        hash_fun(AAA1,MM1);
        for(i=0; i<5; i++) AAA1[i]=AAA1[i] + ACONST[i];
    	n1--;
		}
    	       
       j=0;
       for(i=0; i<5; i++)
       {
       HH[j]= (WORD)AAA1[i] & 0xffff;
       j++;
       qq1= AAA1[i] >> 16;
	   HH[j]=(WORD)qq1 & 0xffff;
       j++;
       }

       HH[10]=0x0000;
        cout <<endl<<"Значение хэш-функции"<<endl<<"H = "; 
        for(i=0; i<11; i++) printf("%04x ",HH[i]); 
		cout<<endl;

        /* 2) Вычисление R компонеты подписи */

		step_mod(PP,g1,len,skl,len,R1);
        mod_p(R1,len,QQ,len);

        cout<<endl<<"-Подпись сообщения-"<<endl<<"Компонента R = "; 
		for(i=0; i<len; i++) printf("%04x ",R1[i]); 
       
          
        /* 3) Вычисление S компоненты подписи */
		
        umn(XX,len,R1,len,X5,2*len);
        mod_p(X5,2*len,QQ,LEN1);
        plus(len,X5,HH,X6);
        mod_p(X6,len,QQ,LEN1);
        obr(len,skl,QQ,M);
        umn(X6,len,M,len,X5,2*len);
        mod_p(X5,2*len,QQ,LEN1);
        memcpy(S1,X5,len*sizeof(WORD));

    	cout<<endl<<"Компонента S = "; 
		for(i=0; i<len; i++) printf("%04x ",S1[i]);
		cout<<endl;
        return;

}
/* Функция выполняет проверку подписи */
void prov_pod(int len, WORD *g1, char *bb, WORD *okl, WORD *R1, WORD *S1)
{
    int i,j,j1,z,z1,n,n1,n2,n3;
    BYTE MM[64];
    WORD M[LEN1],X4[2*LEN1],X5[2*LEN1],X6[LEN1];
    WORD AA[LEN1],BB[LEN1],V[LEN1],HH1[LEN1];
    DWORD qq1,MM2[16],AAA0[5];

        /* 1) Вычисление хэш-функции подписываемых данных */
 
        for(i=0; i<5; i++) AAA0[i]=ACONST[i];
		i=0;
		n=0;
		while (bb[i]!= '\0')
		{
			n++;
			i++;
		}

		cout<<endl<<"Сообщение, переведенное в коды ASCII "<<endl;
         j=0;
         for(i=0; i<n; i++) 
         {
          printf("%02x ",(BYTE)bb[i]);
          j++;  
          if(j==16) { j=0; printf("\n"); }
         }
       
        cout<<endl<<"Количество символов n = "<<n;
		n1=n/64;
		n2=n%64;
        cout<<endl<<"Количество блоков по 64 символа n1 = "<<n1;
		cout<<endl<<"Количество оставшихся символов n2 = "<<n2<<endl;   

       if(n2!=0) n3=n1+1;
       z1=0;

       for(z=0; z<n3; z++)
       {
        if(n1!=0) for(j1=0; j1<64; j1++) { MM[j1]= (BYTE)bb[z1]; z1++;}
            else {
                  for(j1=0; j1<n2; j1++) { MM[j1]= (BYTE)bb[z1]; z1++; }
                  for(j1=n2; j1<64; j1++) MM[j1]= 0x00;
                 }
  
        j=0;
        for(i=0; i<16; i++)
         { 
         qq1= (DWORD)MM[j+3]<<24 ^ (DWORD)MM[j+2]<<16 ^ (DWORD)MM[j+1]<<8; 
         MM2[i]=(DWORD)MM[j] ^qq1;
         j+=4;
         }   

        hash_fun(AAA0,MM2);
        for(i=0; i<5; i++) AAA0[i]=AAA0[i] + ACONST[i];
        n1--;
		}

       j=0;
       for(i=0; i<5; i++)
       {
       HH1[j]= (WORD)AAA0[i] & 0xffff;
       j++;
       qq1= AAA0[i] >> 16;
	   HH1[j]=(WORD)qq1 & 0xffff;
       j++;
       }

       HH1[10]=0x0000;
       cout <<endl<<"Вычисленное значение хэш-функции"<<endl<<"H = ";
        for(i=0; i<11; i++) printf("%04x ",HH1[i]); 
		cout<<endl;

		cout <<endl<<"Вычисление компонент A,B,V ";
		/* 2) Вычисление А */
        obr(len,S1,QQ,M);
        umn(HH1,len,M,len,X5,2*len);
        mod_p(X5,2*len,QQ,len);
        memcpy(AA,X5,len*sizeof(WORD));

        printf("\nA="); 
		for(i=0; i<len; i++) printf("%04x ",AA[i]);

        /* 3) Вычисление В */
        umn(R1,len,M,len,X5,2*len);
        mod_p(X5,2*len,QQ,len);
        memcpy(BB,X5,len*sizeof(WORD));

      	printf("\nB=");
		for(i=0; i<len; i++) printf("%04x ",BB[i]);
        
        /* 4) Вычисление V */
        step_mod(PP,g1,len,AA,len,X5);
        step_mod(PP,okl,len,BB,len,X6);
        umn(X5,len,X6,len,X4,2*len);
        mod_p(X4,2*len,PP,len);
        mod_p(X4,len,QQ,len);
        memcpy(V,X4,len*sizeof(WORD));

       	printf("\nV=");
		for(i=0; i<len; i++) printf("%04x ",V[i]);
         
		cout<<endl<<endl<<"Подпись верна так как вычисленное значение V равно подписанному значению R"<<endl;
		
       	cout <<endl <<endl <<"нажмите любую клавишу... "<<endl; 
        getchar();
        cout<<endl;
		return;
}  

/* Функция вычисляет значение хэш-функции для блока длиной 512 бит */
void hash_fun(DWORD *TT2, DWORD *M2)
{  int i;
   DWORD K1,K2,K3,K4,KK,W[80],W1,temp,r,rez;

        K1=0x5a827999;
        K2=0x6ed9eba1;
        K3=0x8f1bbcdc;
        K4=0xca62c1d6;
		
        for(i=0; i<80; i++)
        {
        if(i>=0 && i <=19)  KK=K1;
        if(i>=20 && i <=39) KK=K2;
        if(i>=40 && i <=59) KK=K3;
        if(i>=60 && i <=79) KK=K4;

        if(i>=0 && i <=15) W[i]=M2[i];
		if(i>=16 && i <=79)  
		  {
			W1=W[i-3] ^ W[i-8] ^ W[i-14] ^ W[i-16];
            W[i]=(W1 >> 31) ^ (W1 << 1);
		  }
        rez = hash_f(TT2[1],TT2[2],TT2[3],i);

         r=(TT2[0] >> 27) ^ (TT2[0] << 5);

         temp=r + rez + TT2[4] + W[i] + KK;
         TT2[4]=TT2[3];
         TT2[3]=TT2[2];
         r=(TT2[1] >> 2) ^ (TT2[1] << 30);
         TT2[2]= r;
         TT2[1]=TT2[0];
         TT2[0]=temp;
   		}
  return;
}

/* Функция вычисляет значение функции f(x,y,z) при вычислениихэш-функции */
int hash_f(DWORD x, DWORD y, DWORD z, int i1)
{
DWORD rez1;

        if(i1>=0 &&  i1 <=19) rez1=(x & y) | ((~x)& z);
        if(i1>=20 && i1 <=39) rez1=x ^ y ^ z;
        if(i1>=40 && i1 <=59) rez1=(x & z) | (x & y) | (z & y);
        if(i1>=60 && i1 <=79) rez1=x ^ y ^ z; 
  return rez1;
}

/* Функция вычитает b из a, результат записывает  в c.
 len - длина числа в словах. */


void minus(int len, WORD * a, WORD * b, WORD * c)
{
 WORD x[LENBUF];

 memcpy(x,b,len*sizeof(WORD));
 change_sign(len,x);
 plus(len,a,x,c);
}

/* Функция вычисляет сумму чисел a и b, результат записывает в c.
 len - длина числа в словах.*/

void plus(int len, WORD * a, WORD * b, WORD * c)
{
 int i,h=0;
 long d;

 for(i=0;i<len;i++)
 {
  //d1=a[i];
  d=(long)a[i]+b[i]+h;
  c[i]=(WORD)d&0xffff;
  h=(d&0x10000l) ? 1 : 0;
 }
}

/* Функция меняет знак числа, т.е. отрицательное число записывает
 в дополнительном коде.
 len - длина числа в словах */

void change_sign(int len, WORD * c)
{
 int i,p;
 long s;

 for(i=0;i<len;i++) c[i]^=0xffff;
 if((c[0]&1)==0) c[0]|=1; else {
                                i=0;
                                p=1;
                                do {
                                    s=(long)c[i]+1;
                                    c[i]=(WORD)s&0xffff;
                                    if(s&0x10000l) p=1; else p=0;
                                    i++;
                                   }
                                while(p==1 && i<len);
                               }
}

/* Функция выполняет поиск номера старшей единицы.
 Возвращает: номер найденной 1,
             -1  - нет единиц.
 len - длина числа в словах */

int num_first_1(int len, WORD * a)
{
 int n=0,i,j,ret;

 for(i=0;i<len;i++)
 {
  if(!a[len-i-1]) n+=16;  
     else for(j=15;j>=0;j--) if(a[len-i-1]>>j){
                                               ret = (len<<4)-n-1;
	                                       goto end;
	                                      }
	                        else n++;
 }
 ret= (len<<4)-n-1;
 end: return ret;
}


/* Функция делит a на b (в целых числах), результат записывает в c.
 len - длина числа в словах. */
int Div(int len, WORD * a, WORD * b, WORD * c)
{
 WORD y[LENBUF];
 int ret;

 ret=divost(len,a,b,c,y);
 return ret;
}

/* Функция делит a на b (в целых числах), результат записывает в
 c, остаток записывает в r.
 len - длина числа в словах.*/
int divost(int len, WORD * a, WORD * b, WORD * c, WORD * r)
{
 WORD y[LENBUF],yb[LENBUF];
 int sign1,sign2,m1,m2,kn;

 memcpy(y,a,len*sizeof(WORD));
 memcpy(yb,b,len*sizeof(WORD));
 memset(c,0,len*sizeof(WORD));
 if(!memcmp(c,yb,len*sizeof(WORD))) return -1;
 sign1 = (y[len-1]>>15)&1;
 sign2 = (yb[len-1]>>15)&1;
 if(sign1) change_sign(len,y);
 if(sign2) change_sign(len,yb);
 if(cmp(len,y,yb)!=1)
 {
  WORD x[LENBUF];
  m2=num_first_1(len,yb);
  do {
      m1=num_first_1(len,y);
      kn=m1-m2;
      mul2(kn,len,yb,x);
      if(cmp(len,y,x)==1) {
                           kn--;
                           mul2(kn,len,yb,x);
                          }
      c[(kn>>4)&0xff]|=1<<(kn&0xf);
      minus(len,y,x,y);
    }
  while(cmp(len,y,yb)!=1);
 }
 memcpy(r,y,len*sizeof(WORD));
 return 0;
}

/* Функция выполняет сдвиг длинного числа а влево на deg бит.
 Результат записывает в с.*/
void mul2(int deg, int len, WORD * a, WORD * c)
{
 WORD x[LENBUF];
 int i,k1,k2;

 k1=deg>>4;
 k2=deg&0xf;
 memset(x,0,len*sizeof(WORD));
 for(i=0;i<len;i++)
 {
  if(i+k1+1<len) x[i+k1+1]|=(a[i]>>(16-k2));
  if(i+k1<len) x[i+k1]|=(a[i]<<k2);
 }
 memcpy(c,x,len*sizeof(WORD));
}

/* Функция определяет какое из двух беззнаковых чисел, представленных
 в массивах a и b, больше:
      0 - первое,
      1 - второе,
     -1 - одинаковые.
 len - длина числа в словах. */
int cmp(int len, WORD * a, WORD * b)
{
 int ret,m1,m2,i;

 m1=num_first_1(len,a);
 m2=num_first_1(len,b);
 if(m1>m2) ret = 0; else 
    if(m1<m2) ret = 1; else {
                             i=m1>>4;
                             while((a[i]==b[i]) && i>=0) i--;
                             if(i<0) ret = -1;
                                else if(a[i]>b[i]) ret = 0;
                                        else ret = 1;
                            }
 return ret;
}

/* Функция вычисляет c - обратное к a по модулю N.
 len - длина числа в словах.
 Возвращает -1, если обратное к нулю,
             0 - в остальных случаях.*/

int obr(int len, WORD * a, WORD * N, WORD * c)
{
 int r;
 WORD v[LENBUF],u[LENBUF],v0[LENBUF],u0[LENBUF],q[LENBUF],x[LENBUF*2];

 if(num_first_1(len,a)==-1)
 {
  memset(c,0,len*sizeof(WORD));
  return -1;
 }
 memset(u0,0,len*sizeof(WORD));
 memset(u,0,len*sizeof(WORD));
 u[0]=1;
 memcpy(v0,N,len*sizeof(WORD));
 memcpy(v,a,len*sizeof(WORD));
 Div(len,N,a,q);
 while((r=num_first_1(len,v))>0)
 {
  memcpy(c,v,len*sizeof(WORD));
  umn(q,len,v,len,x,2*len);
  minus(len,v0,x,v);
  memcpy(v0,c,len*sizeof(WORD));

  memcpy(c,u,len*sizeof(WORD));
  umn(q,len,u,len,x,2*len);
  minus(len,u0,x,u);
  memcpy(u0,c,len*sizeof(WORD));

  Div(len,v0,v,q);
 }
 if((u[len-1]>>15)&1) plus(len,u,N,c);
     else memcpy(c,u,len*sizeof(WORD));

 if(r==0) return 0; else return 1;
}

/* Функция выполняет умножение двух чисел, представленных в массивах
 a и b. Результат помещается в c.*/
void umn(WORD *a,int n1 ,WORD *b,int n2,WORD *c,int n3)
{
 WORD a1[LENBUF];
 WORD b1[LENBUF];
 unsigned long d;
 int i,j;
 memcpy(a1,a,n1*sizeof(WORD));
 memcpy(b1,b,n2*sizeof(WORD));
 memset(c,0,n3*sizeof(WORD));

 for(i=0;i<n2 && i<n3;i++)
 {
  int h=0;
  for(j=0;j<n1 && (i+j)<n3;j++)
  {
   d=(unsigned long)a1[j]*b1[i]+h+c[i+j];
   c[i+j]=(WORD)d&0xffff;
   h=d>>16;
   if(j==n1-1 && (i+j+1)<n3) {
	                      d=d>>16;
	                      c[i+j+1]=(WORD)d&0xffff;
                             }
  }
 }
}

// a по модулю p
void mod_p(WORD * a, WORD n1, WORD * p, WORD n2)
{
  int m1=num_first_1(n1,a);
  int m2=num_first_1(n2,p);
  int len;
  WORD  a1[LENBUF];
  WORD  c[LENBUF];
  WORD  p1[LENBUF];
  if(m2>m1)
    return;
  else if(m1==m2)
  {
    if(cmp(min(n1,n2),a,p)==1)
      return;
  }
  len=max(n1,n2);

  memset(p1,0,len*sizeof(WORD));
  memcpy(p1,p,n2*sizeof(WORD));
  memset(a1,0,len*sizeof(WORD));
  memcpy(a1,a,n1*sizeof(WORD));
  do
  {
    int m1=num_first_1(len,a1);
    int kn=m1-m2;
    mul2(kn,len,p1,c);
    if(cmp(len,a1,c)==1)
    {
      kn--;
      mul2(kn,len,p1,c);
    }
    minus(len,a1,c,a1);
  }
  while(cmp(len,a1,p1)!=1);
  memcpy(a,a1,n1*sizeof(WORD));
}

// возведение в степень
void step_mod(WORD * p, WORD * a, int n1, WORD * b, int n2, WORD * c)
{
	WORD c1[LEN1];
	WORD buf[2*LEN1];
	int i,j;
	for(i=0; i<n1; i++) c1[i] = 0;
	c1[0]=1;
	for(i=n2-1; i>=0; i--) 
        {
 	 for(j=15; j>=0; j--)
	 {
 	  umn(c1,n1,c1,n1,buf,2*n1);
	  mod_p(buf,2*n1,p,n1);
          memcpy(c1,buf,n1*sizeof(WORD));

          if((b[i]>>j)&1) 
          {  
	   umn(a,n1,c1,n1,buf,2*n1);
	   mod_p(buf,2*n1,p,n1);
	   memcpy(c1,buf,n1*sizeof(WORD));
	  }
        }
      }
	memcpy(c,c1,n1*sizeof(WORD));
}




