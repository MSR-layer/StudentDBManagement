#include <stdio.h>
#include <stdlib.h>

int KAYITSAYISI =  0;
int foffset = 0;

void indexDosyaOlustur();
void kayitEkle();
void kayitBul();
void kayitSil();
void kayitGuncelle();
void veriDosyasiniGoster();
void indexDosyasiniGoster();
void indexDosyasiniSil();
void kayitSayisiBul();

typedef struct kayit{
    int ogrNo;
    int dersKodu;
    int puan;
    int offset;

}kayit;

int main()
{
    kayitSayisiBul();
    foffset = KAYITSAYISI * sizeof(kayit);

    int secim;
    do{
        printf("\n1. INDEX DOSYASI OLUSTUR\n");
        printf("2. KAYIT EKLE\n");
        printf("3. KAYIT BUL\n");
        printf("4. KAYIT SIL\n");
        printf("5. KAYIT GUNCELLE\n");
        printf("6. VERI DOSYASI GOSTER\n");
        printf("7. INDEX DOSYASI GOSTER\n");
        printf("8. INDEX DOSYASINI SIL\n");
        printf("0. CIKMAK\n");
        printf("\nislem seciniz: ");
        scanf("%d", &secim);

        switch(secim){
            case 1:
                indexDosyaOlustur();
                break;
            case 2:
                kayitEkle();
                break;
            case 3:
                kayitBul();
                break;
            case 4:
                kayitSil();
                break;
            case 5:
                kayitGuncelle();
                break;
            case 6:
                veriDosyasiniGoster();
                break;
            case 7:
                indexDosyasiniGoster();
                break;
            case 8:
                indexDosyasiniSil();
                break;


            }
    }while(secim!=0);


    exit(1);


    return 0;
}


void indexDosyaOlustur()
{
    kayitSayisiBul();
    FILE* fp = fopen("veriDosyasi.bin","rb");
    FILE* fx = fopen("indexDosyasi.txt","w");

    kayit buffer;
    kayit * kayitlar;
    kayitlar = (kayit *)calloc(KAYITSAYISI,sizeof(kayit));
    int i=0;
    while(fread(&buffer,sizeof(kayit),1,fp))
    {
        kayitlar[i] = buffer;
        i++;
    }

    for(i=0;i<KAYITSAYISI;i++)
    {
        for(int j=i+1;j<KAYITSAYISI;j++)
        {
            if(kayitlar[i].ogrNo>kayitlar[j].ogrNo)
            {
                kayit temp = kayitlar[i];
                kayitlar[i] = kayitlar[j];
                kayitlar[j] = temp;
            }
        }
    }

    for(i = 0;i<KAYITSAYISI;i++)
    {
        fprintf(fx,"%d %d\n",kayitlar[i].ogrNo,kayitlar[i].offset);
    }

    fclose(fx);
    fclose(fp);
}

void kayitEkle()
{
    FILE * fp = fopen("veriDosyasi.bin","ab");
    FILE * fx = fopen("indexDosyasi.txt","r");

    kayit *s;
    int n;
    printf("kac kayit eklemek istiyorsunuz: ");
    scanf("%d", &n);

    kayit buffer;
    for(int i=0;i<n;i++)
    {
        printf("Ogrenci numarasi giriniz: ");
        scanf("%d",&buffer.ogrNo);
        printf("Ders kodu giriniz: ");
        scanf("%d",&buffer.dersKodu);
        printf("Puan giriniz: ");
        scanf("%d",&buffer.puan);
        buffer.offset = foffset;
        fwrite(&buffer,sizeof(kayit),1,fp);
        foffset += sizeof(kayit);
    }

    fclose(fp);
    kayitSayisiBul();

    if(fx != NULL)
    {
        fclose(fx);
        indexDosyaOlustur();

    }else
    {
        printf("indeks dosyasi olusturulmamistir. Olusturmak icin 1'e basiniz.\n");
    }


}

void kayitBul()
{
    kayitSayisiBul();

    FILE* fx = fopen("indexDosyasi.txt","r");
    FILE* fv = fopen("veriDosyasi.bin", "rb");

    if(fv == NULL)
    {
        printf("veri dosyasi bulunmamaktadir...\n");
        return;
    }
    if(fx == NULL)
    {
        printf("index dosyasi bulunmamaktadir...\n");
        return;
    }

    int aranan;
    printf("aranacak kaydin ogrenci numarasini giriniz: ");
    scanf("%d", &aranan);

    int L = 0,R = KAYITSAYISI-1;
    int M = 0;
    int i=0;

    int offset;
    while(L<=R)
    {
        M = (L+R)/2;
        fseek(fx,0,SEEK_SET);

        for(int k=0;k<M;k++)
        {
            char ch = fgetc(fx);
            while(ch !='\n')
            {
                ch = fgetc(fx);
            }
        }

        fscanf(fx,"%d %d",&i, &offset);
        if(i == aranan)
        {
            break;
        }else if(i<aranan)
        {
            L = M + 1;
        }else if(i>aranan)
        {
            R = M - 1;
        }
        i=0;
    }

    int up = M, down = M;

    kayit buffer;
    if(i!=0)
    {
        fseek(fv,offset,SEEK_SET);
        fread(&buffer,sizeof(kayit),1,fv);
        printf("\t\t%d %d %d\n",buffer.ogrNo,buffer.dersKodu,buffer.puan);
        while(up-1 >= 0)
        {
            fseek(fx,0,SEEK_SET);

            for(int k=0;k<up-1;k++)
            {
                char ch = fgetc(fx);
                while(ch !='\n')
                {
                    ch = fgetc(fx);
                }
            }

            fscanf(fx,"%d %d",&i, &offset);
            if(i == aranan)
            {
                fseek(fv,offset,SEEK_SET);
                fread(&buffer,sizeof(kayit),1,fv);
                printf("\t\t%d %d %d\n",buffer.ogrNo,buffer.dersKodu,buffer.puan);
                up--;
            }
            else
                break;
        }

        while(down+1<=KAYITSAYISI)
        {
            fseek(fx,0,SEEK_SET);

            for(int k=0;k<down+1;k++)
            {
                char ch = fgetc(fx);
                while(ch !='\n')
                {
                    ch = fgetc(fx);
                }
            }

            fscanf(fx,"%d %d",&i, &offset);
            if(i == aranan)
            {
                fseek(fv,offset,SEEK_SET);
                fread(&buffer,sizeof(kayit),1,fv);
                printf("\t\t%d %d %d\n",buffer.ogrNo,buffer.dersKodu,buffer.puan);
                down++;
            }
            else
                break;
        }
    }else
    {
        printf("%d nolu kayit bulunamadi.\n", aranan);
    }

    fclose(fx);
    fclose(fv);
}

void kayitSil()
{
    kayitSayisiBul();
    FILE* fvread = fopen("veriDosyasi.bin", "rb");

    if(fvread == NULL)
    {
        printf("veri dosyasi bulunmamaktadir...\n");
        return;
    }

    int silinecek;
    printf("Silmek istediginiz kaydin ogrenci numarasini giriniz: ");
    scanf("%d", &silinecek);

    kayit * a = (kayit *)calloc(KAYITSAYISI,sizeof(kayit));

    kayit buffer;
    int j=0;
    while(fread(&buffer,sizeof(kayit),1,fvread) == 1)
    {
        if(buffer.ogrNo == silinecek)
        {
            continue;
        }
        a[j] = buffer;

        j++;
    }

    fclose(fvread);

    FILE* fvwrite = fopen("veriDosyasi.bin","wb");

    int i;
    for(i = 0;i<KAYITSAYISI;i++)
    {
        if(a[i].ogrNo==0)
            continue;
        a[i].offset = i*sizeof(kayit);
        fwrite(&a[i],sizeof(kayit),1,fvwrite);
    }
    fclose(fvwrite);

    FILE* fxwrite = fopen("indexDosyasi.txt","w");

    kayitSayisiBul();
    for(i=0;i<KAYITSAYISI;i++)
    {
        for(j=i+1;j<KAYITSAYISI;j++)
        {
            if(a[i].ogrNo>a[j].ogrNo)
            {
                kayit temp = a[i];
                a[i] = a[j];
                a[j] = temp;
            }
        }
    }

    for(i = 0;i<KAYITSAYISI;i++)
    {
        if(a[i].ogrNo == 0)
                continue;
        fprintf(fxwrite,"%d %d\n",a[i].ogrNo,a[i].offset);
    }


    fclose(fxwrite);

}

void kayitGuncelle()
{
    kayitSayisiBul();
    FILE * fv = fopen("veriDosyasi.bin", "rb+");
    FILE * fx = fopen("indexDosyasi.txt", "r");

    if(fv == NULL)
    {
        printf("veri dosyasi bulunmamaktadir...\n");
        return;
    }
    if(fx == NULL)
    {
        printf("index dosyasi bulunmamaktadir...\n");
        return;
    }

    int numara, derskodu;
    printf("Puani guncellemek istediginiz kayidin numarasini ve ders kodunu giriniz: ");
    scanf("%d %d",&numara,&derskodu);


    int L = 0,R = KAYITSAYISI-1;
    int M = 0;
    int i=0;

    int bulunan;
    int offset;
    while(L<=R)
    {
        M = (L+R)/2;
        fseek(fx,0,SEEK_SET);

        for(int k=0;k<M;k++)
        {
            char ch = fgetc(fx);
            while(ch !='\n')
            {
                ch = fgetc(fx);
            }
        }

        fscanf(fx,"%d %d",&i, &offset);
        if(i == numara)
        {
            break;
        }else if(i<numara)
        {
            L = M + 1;
        }else if(i>numara)
        {
            R = M - 1;
        }
        i=0;
    }

    fseek(fv,0,SEEK_SET);
    kayit buffer;
    int up = M, down = M;
    if(i!=0)
    {
        fseek(fv,offset,SEEK_SET);
        fread(&buffer,sizeof(kayit),1,fv);
        if(buffer.dersKodu == derskodu)
        {
            fseek(fv,offset,SEEK_SET);
            int yeniPuan;
            printf("Yeni puani giriniz: ");
            scanf("%d",&yeniPuan);
            buffer.puan = yeniPuan;
            fwrite(&buffer,sizeof(kayit),1,fv);
            printf("\t\t%d numarali ders kodu %d olan kaydi guncellenmistir... ",numara ,derskodu);
        }else
        {

            while(up - 1 >= 0)
            {
                 fseek(fx,0,SEEK_SET);

                for(int k=0;k<up-1;k++)
                {
                    char ch = fgetc(fx);
                    while(ch !='\n')
                    {
                        ch = fgetc(fx);
                    }
                }

                fscanf(fx,"%d %d",&i, &offset);
                if(i == numara)
                {
                    fseek(fv,offset,SEEK_SET);
                    fread(&buffer,sizeof(kayit),1,fv);
                    if(buffer.dersKodu == derskodu)
                    {
                        fseek(fv,offset,SEEK_SET);
                        int yeniPuan;
                        printf("Yeni puani giriniz: ");
                        scanf("%d",&yeniPuan);
                        buffer.puan = yeniPuan;
                        fwrite(&buffer,sizeof(kayit),1,fv);
                        printf("\t\t%d numarali ders kodu %d olan kaydi guncellenmistir... ",numara ,derskodu);
                    }
                    up--;
                }
                else
                    break;
            }

            while(down + 1 <= KAYITSAYISI)
            {

                fseek(fx,0,SEEK_SET);

                for(int k=0;k<down+1;k++)
                {
                    char ch = fgetc(fx);
                    while(ch !='\n')
                    {
                        ch = fgetc(fx);
                    }
                }

                fscanf(fx,"%d %d",&i, &offset);
                if(i == numara)
                {
                    fseek(fv,offset,SEEK_SET);
                    fread(&buffer,sizeof(kayit),1,fv);
                    if(buffer.dersKodu == derskodu)
                    {
                        fseek(fv,offset,SEEK_SET);
                        int yeniPuan;
                        printf("Yeni puani giriniz: ");
                        scanf("%d",&yeniPuan);
                        buffer.puan = yeniPuan;
                        fwrite(&buffer,sizeof(kayit),1,fv);
                        printf("\t\t%d numarali ders kodu %d olan kaydi guncellenmistir... ",numara ,derskodu);
                    }
                    down++;
                }
                else
                    break;
            }
        }
    }else
    {
        printf("\t\t%d nolu kayit bulunamadi.\n", numara);
    }

    fclose(fx);
    fclose(fv);
}

void veriDosyasiniGoster()
{
    FILE* fv = fopen("veriDosyasi.bin", "rb");

    if(fv == NULL)
    {
        printf("veri dosyasi bulunmamaktadir...\n");
        exit(1);
    }

    kayit buffer;
    while(fread(&buffer,sizeof(kayit),1,fv))
    {
        printf("\t\t%-3d %-3d %-3d\n",buffer.ogrNo,buffer.dersKodu,buffer.puan);
    }
    fclose(fv);
}

void indexDosyasiniGoster()
{
    FILE* fx = fopen("indexDosyasi.txt","r");

    if(fx == NULL)
    {
        printf("index dosyasi bulunmamaktadir...\n");
        return;
    }

    printf("\t\t");
    char ch = fgetc(fx);

    while(ch != EOF)
    {
        putchar(ch);
        if(ch == '\n')
            printf("\t\t");
        ch = fgetc(fx);
    }

    fclose(fx);
}

void indexDosyasiniSil()
{
    remove("indexDosyasi.txt");
}

void kayitSayisiBul()
{
    FILE* fv = fopen("veriDosyasi.bin", "rb");
    kayit buffer;
    while(fread(&buffer,sizeof(buffer),1,fv) == 1)
    {
        KAYITSAYISI += sizeof(kayit);
    }

    KAYITSAYISI = KAYITSAYISI/sizeof(kayit);
    fclose(fv);
}
