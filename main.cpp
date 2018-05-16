#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <ctype.h>
#include <locale.h>
#include <string.h>
#include <allegro.h>

#define INFINITY 9999
int MAX=0;


char dizim[22][50];

char sehir_adi[100][50];
char sehir_kodu[100][10];

char *sehir1[300][1];
char *sehir2[300][1];
int mesafe[300][1];

char koordinat_sehir_isim[81][40];
int koordinat_rakam[81][2];
char harita_sehir_dizi[1200];

int sehir_sayisi=0;
int mesafe_sayisi=0;
int matris[81][81];
char iladi[20];



void AllegroBaslat(){
    int depth, res;
    allegro_init();
    depth = desktop_color_depth();
    if (depth == 0) depth = 32;
    set_color_depth(depth);
    res = set_gfx_mode(GFX_AUTODETECT_WINDOWED, 960, 450, 0, 0);
    if (res != 0) {
        allegro_message(allegro_error);
        exit(-1);
    }
    install_timer();
    install_keyboard();
    install_mouse();
}
void AllegroBitir(){
    clear_keybuf();
}
void KoordinatDosyaOku(){
    FILE *fp;
    char satir[40];
    char sehir_koordinatlari[1200];
    char *ayir;
    int i=0,il_sayisi=0;
    if(fopen("koordinatlar.txt","r")){
        fp=fopen("koordinatlar.txt","r");
        printf("\n");
        while(!feof(fp)){
            fscanf(fp,"%s",satir);
            //printf("satir hata %d  %s\n",i,satir);
            strcat(satir,"\n");
            strcat(sehir_koordinatlari,satir);
            strcpy(satir,"");
            i++;
            if(i==70)
                break;
        }
        fclose(fp);
        ayir=strtok(sehir_koordinatlari, "-\n");
        for(i=0;ayir!=NULL;i++){
            il_sayisi++;
            strcpy(koordinat_sehir_isim[i],ayir);
            ayir=strtok(NULL,"-\n");
            koordinat_rakam[i][0]=atoi(ayir);
            ayir=strtok(NULL,"-\n");
            koordinat_rakam[i][1]=atoi(ayir);
            ayir=strtok(NULL,"-\n");
        }
    }
    else{
        printf("Dosya okunamadi..\n");
    }
}

int KoordinatGetir(char gelen_sehir[40]){
    int i;
    /*
    char *gelen=gelen_sehir;
    char *gelen2=koordinat_sehir_isim[13];
    */
    //printf("\ngelen sehir : %s\n",gelen_sehir);
    //printf("gelen sehir : %s\n",koordinat_sehir_isim[0]);
    //printf("gelen sehir : %s\n\n",koordinat_sehir_isim[13]);
    for(i=0;i<81;i++){
        if(strcmp(koordinat_sehir_isim[i],gelen_sehir)==0){
            return i;
        }
    }
    return 0;
}

void AllegroMain(char sehir_yol[1200]){
    char *ayir;
    int i,j,index;
    int gelen_koordinatlar[50][2];
    char ayrilan_sehir[40];
    ayir=strtok(sehir_yol, "-\n");

    for(i=0;ayir!=NULL;i++){
        strcpy(ayrilan_sehir,ayir);
        index=KoordinatGetir(ayrilan_sehir);
        //printf("%d index ",index);
        gelen_koordinatlar[i][0]=koordinat_rakam[index][0];
        gelen_koordinatlar[i][1]=koordinat_rakam[index][1];
        //printf("%s %d-%d i : %d\n",koordinat_sehir_isim[index],gelen_koordinatlar[i][0],gelen_koordinatlar[i][1],i);
        //printf("sehirimiz : %s\n",koordinat_sehir_isim[1]);
        ayir=strtok(NULL,"-\n");
    }
    //printf("en son bu %d",i);

    AllegroBaslat();
    BITMAP *arka_plan_resmi=load_bitmap("turkiye_haritasi.bmp",NULL);
    show_mouse(screen);
    while (!key[KEY_ESC]){
        acquire_screen();
        blit(arka_plan_resmi,screen,0,0,0,0,1100,450);
        //textprintf_ex(screen,font,250,50,makecol(250,250,250),makecol(0, 0, 0),"%d",mouse_x);
        //textprintf_ex(screen,font,250,60,makecol(250,250,250),makecol(0, 0, 0),"%d",mouse_y);
        for(j=0;j<i-1;j++){
            //textprintf_ex(screen,font,250,70,makecol(250,250,250),makecol(0, 0, 0),"%d - %d",gelen_koordinatlar[j][0],gelen_koordinatlar[j][1]);
            char cikis_mesaj[]="Cikis yapmak icin ESC tusuna basin...";
            textprintf_ex(screen,font,650,20,makecol(250,250,250),makecol(0, 0, 0),"%s",cikis_mesaj);
            line(screen,gelen_koordinatlar[j][0],gelen_koordinatlar[j][1],gelen_koordinatlar[j+1][0],gelen_koordinatlar[j+1][1],makecol(60,60,60));

            floodfill(arka_plan_resmi, gelen_koordinatlar[j][0], gelen_koordinatlar[j][1], makecol(160,60,60));
            floodfill(arka_plan_resmi, gelen_koordinatlar[j+1][0], gelen_koordinatlar[j+1][1], makecol(160,60,60));
        }
        release_screen();
    }
    AllegroBitir();
}

void dijkstra(int TumSehirlerMatrisi[81][81],int n,int startnode,int targetnode);

void SehirOku(){
    FILE *fp;
    char str_sehir[150];
    char str_sehirler[1200];
    char *ayir;
    int i;

    if(fopen("sehir_kod.txt","r")){
        fp=fopen("sehir_kod.txt","r");
        while(!feof(fp)){
            fscanf(fp,"%s",str_sehir);
            strcat(str_sehir,"\n");
            strcat(str_sehirler,str_sehir);
        }
        fclose(fp);

        ayir=strtok(str_sehirler,"-\n");
        ayir=strtok(NULL,"-\n");
        ayir=strtok(NULL,"-\n");

        for(i=0;ayir!=NULL;i++){
            sehir_sayisi++;
            strcpy(sehir_adi[i],ayir);
            ayir=strtok(NULL,"-\n");
            strcpy(sehir_kodu[i],ayir);
            ayir=strtok(NULL,"-\n");
        }
    }
    else{
        printf("Dosya okunamadi..\n");
    }
}

void MesafeOku(){
    FILE *fp;
    char str_sehir_mesafe[150];
    char str_sehirler_mesafe[2500];
    char *ayir;
    int i;
    if(fopen("sehir_mesafe.txt","r")){
        fp=fopen("sehir_mesafe.txt","r");
        while(!feof(fp)){
            fscanf(fp,"%s",str_sehir_mesafe);
            strcat(str_sehir_mesafe,"\n");
            strcat(str_sehirler_mesafe,str_sehir_mesafe);
        }
        fclose(fp);
        ayir=strtok(str_sehirler_mesafe,"-\n");
        ayir=strtok(NULL,"-\n");
        ayir=strtok(NULL,"-\n");
        ayir=strtok(NULL,"-\n");
        for(i=0;ayir!=NULL;i++){
            mesafe_sayisi++;
            sehir1[i][0]=ayir;
            ayir=strtok(NULL,"-\n");
            sehir2[i][0]=ayir;
            ayir=strtok(NULL,"-\n");
            mesafe[i][0]=atoi(ayir);
            ayir=strtok(NULL,"-\n");
        }
    }
    else{
        printf("Dosya okunamadi..\n");
    }
}


int IlKoduBul(char *kaynak){
    int i;
    int ilKodu;
    for(i=0;i<sehir_sayisi;i++){
        if(strcmp(kaynak,sehir_adi[i])==0){
            ilKodu=i;
            break;
        }
    }
    return ilKodu;
}

int IlMesafeleriBul(char *kaynak, char *hedef){
    int i;
    int kaynak_id;
    for(i=0;i<mesafe_sayisi;i++){
        if((strcmp(kaynak,sehir1[i][0])==0) && (strcmp(hedef,sehir2[i][0])==0)){
            kaynak_id=i;
        }
    }
    return kaynak_id;
}

void SehirListele(){
    int i;
    for(i=0;i<sehir_sayisi;i++){
        //printf("sehir : %s , il kodu : %s\n",sehir_adi[i],sehir_kodu[i]);
        strcpy(dizim[i],sehir_adi[i]);
    }
}

void MesafeListele(){
    int i,j,x,y;
    for(i=0;i<mesafe_sayisi;i++){
        printf("1.sehir : %s , 2.sehir : %s , Mesafe : %d\n",sehir1[i][0],sehir2[i][0],mesafe[i][0]);
        x=IlKoduBul(sehir1[i][0]);
        printf("%d -",x);
        y=IlKoduBul(sehir2[i][0]);
        printf(" %d\n",y);
    }

}

void MatrisiListele(){
    int i,j;
    for(i=0;i<sehir_sayisi;i++){
        for(j=0;j<sehir_sayisi;j++){
            printf("%4d",matris[i][j]);
        }
        printf("\n");
    }
}

void MatrisiSifirla(){
    int i,j;
    for(i=0;i<sehir_sayisi;i++){
        for(j=0;j<sehir_sayisi;j++){
            matris[i][j]=0;
        }
    }
}

void MatrisiOlustur(){
    int i,x,y;
    for(i=0;i<mesafe_sayisi;i++){
        x=IlKoduBul(sehir1[i][0]);
        y=IlKoduBul(sehir2[i][0]);
        matris[x][y]=mesafe[i][0];
        matris[y][x]=mesafe[i][0];
    }
}
int plaka_baslangic;
void HedefAl(){
    printf("Iki sehir arasinda en kisa mesafeyi bulan program...\n\n");
    //int plaka_baslangic;
    int plaka_hedef;
    char girilen_baslangic[10],girilen_hedef[10];
    int i,j,girilen_kontrol=0;
    for(i=0;i<sehir_sayisi;i++){
        if(i%2==1){
            printf("Plaka : %11s,  Sehir : %s",sehir_kodu[i],sehir_adi[i]);
            printf("\n");
        }
        else{
            printf("Plaka : %11s,  Sehir : %.20s\t   |\t  ",sehir_kodu[i],sehir_adi[i]);
        }
    }
    printf("\n\nBaslangic il kodu : ");
    scanf("%s",girilen_baslangic);
    printf("Hedef il kodu : ");
    scanf("%s",girilen_hedef);
    if(atoi(girilen_baslangic) && atoi(girilen_hedef)){
        plaka_baslangic=atoi(girilen_baslangic);
        plaka_baslangic--;
        plaka_hedef=atoi(girilen_hedef);
        plaka_hedef--;
    }
    else{
        while(!girilen_kontrol){
            printf("Harf girilmiz! Sehir kodu giriniz.\n\n");
            printf("Baslangic il kodu : ");
            scanf("%s",girilen_baslangic);
            printf("Hedef il kodu : ");
            scanf("%s",girilen_hedef);
            if(atoi(girilen_baslangic) && atoi(girilen_hedef)){
                girilen_kontrol=1;
                plaka_baslangic=atoi(girilen_baslangic);
                plaka_baslangic--;
                plaka_hedef=atoi(girilen_hedef);
                plaka_hedef--;
            }
        }
    }

    printf("\n");
    //printf("Baslangic Sehir : %s",sehir_adi[plaka_baslangic]);
    MAX=sehir_sayisi;
    dijkstra(matris,MAX,plaka_baslangic,plaka_hedef);


    if(plaka_baslangic==plaka_hedef){
        printf("Farkli sehir kodlari girmelisiniz!");
    }
    else if((plaka_baslangic>=0 && plaka_baslangic<sehir_sayisi) && (plaka_hedef>=0 && plaka_hedef<sehir_sayisi)){
        int secim_kullanici;
        printf("\n\n\nHaritada gormek icin :  1 tusu\n");
        printf("Tum sehirlere mesafe listele :  2 tusu\n");
        printf("Cikis yapmak icin :  3 tusu\n\n");
        printf("Seciminiz : ");
        scanf("%d",&secim_kullanici);

        if(secim_kullanici==1){
            KoordinatDosyaOku();
            AllegroMain(harita_sehir_dizi);
        }
        else if(secim_kullanici==2){
            for(i=0;i<sehir_sayisi;i++){
                if(i!=plaka_baslangic)
                dijkstra(matris,MAX,plaka_baslangic,i);
            }
        }
        else if(secim_kullanici==3){
            exit(0);
        }
        else{
            printf("Yanlis Secim!");
        }
    }
    else{
        printf("\nsehirler mevcut degil.\n");
    }
}




int main()
{
    setlocale(LC_ALL,"Turkish");

    SehirOku();
    MesafeOku();
    SehirListele();
    //MesafeListele();

    MatrisiSifirla();
    MatrisiOlustur();
    //MatrisiListele();

    HedefAl();
    return 0;
}
END_OF_MAIN();

/*
bu fonksiyon yazilirken http://www.thecrazyprogrammer.com/2014/03/dijkstra-algorithm-for-finding-shortest-path-of-a-graph.html
adresinden yararlanilmistir.
*/
void dijkstra(int TumSehirlerMatrisi[81][81],int n,int kaynak_dugum, int hedef_dugum)
{
    int km_matris[MAX][MAX],mesafe_dizi[MAX],tahmin_dizi[MAX];
    int ziyaret_dizi[MAX],sayici,minimum_km,siradaki_dugum,i,j;
    for(i=0;i<n;i++)
    {
        for(j=0;j<n;j++)
        {
            if(TumSehirlerMatrisi[i][j]==0)
            {
                km_matris[i][j]=INFINITY;
            }
            else
            {
                km_matris[i][j]=TumSehirlerMatrisi[i][j];
            }
        }
    }


    for(i=0;i<n;i++)
    {
        mesafe_dizi[i]=km_matris[kaynak_dugum][i];
        tahmin_dizi[i]=kaynak_dugum;
        ziyaret_dizi[i]=0;
    }

    mesafe_dizi[kaynak_dugum]=0;
    ziyaret_dizi[kaynak_dugum]=1;
    sayici=1;

    while(sayici<n-1)
    {
        minimum_km=INFINITY;
        for(i=0;i<n;i++)
        {
            if(mesafe_dizi[i]<minimum_km&&!ziyaret_dizi[i])
            {
                minimum_km=mesafe_dizi[i];
                siradaki_dugum=i;
            }
        }
        ziyaret_dizi[siradaki_dugum]=1;
        for(i=0;i<n;i++){
            if(!ziyaret_dizi[i])
            {
                if(minimum_km+km_matris[siradaki_dugum][i]<mesafe_dizi[i])
                {
                    mesafe_dizi[i]=minimum_km+km_matris[siradaki_dugum][i];
                    tahmin_dizi[i]=siradaki_dugum;
                }
            }
        }
        sayici++;
    }
    //printf("n: %d\n",n);
    for(i=0;i<n;i++)
    {
        if(i!=kaynak_dugum && i==hedef_dugum)
        {
            //printf("geldi\n");
            char yedek_sehir[1200];
            if(hedef_dugum==1){
                if(mesafe_dizi[i]!=9999){
                    printf("\nBaslangic Sehir : %s",sehir_adi[plaka_baslangic]);
                    printf("\nHedef Sehir : %s\n",dizim[i]);
                    printf("Mesafe : %d\n",mesafe_dizi[i]);
                    printf("Gecilen Sehirler : %s",dizim[i]);
                    strcat(harita_sehir_dizi,dizim[i]);
                    strcat(harita_sehir_dizi,"-");
                    j=i;
                    while(j!=kaynak_dugum){
                        j=tahmin_dizi[j];
                        printf(" <- %s",dizim[j]);
                        strcpy(yedek_sehir,harita_sehir_dizi);
                        strcpy(harita_sehir_dizi,dizim[j]);
                        strcat(harita_sehir_dizi,"-");
                        strcat(harita_sehir_dizi,yedek_sehir);
                    }
                    printf("\n");
                }
                else{
                    printf("Secilen sehirler arasi baglanti bulunmuyor!");
                }
            }
            else{
                if(mesafe_dizi[i]!=9999){
                    printf("\nBaslangic Sehir : %s",sehir_adi[plaka_baslangic]);
                    printf("\nHedef Sehir : %s\n",dizim[i]);
                    printf("Mesafe : %d km\n",mesafe_dizi[i]);
                    printf("Gecilen Yollar : %s",dizim[i]);
                    strcat(harita_sehir_dizi,dizim[i]);
                    strcat(harita_sehir_dizi,"-");
                    j=i;
                    while(j!=kaynak_dugum){
                        j=tahmin_dizi[j];
                        printf(" <- %s",dizim[j]);
                        strcpy(yedek_sehir,harita_sehir_dizi);
                        strcpy(harita_sehir_dizi,dizim[j]);
                        strcat(harita_sehir_dizi,"-");
                        strcat(harita_sehir_dizi,yedek_sehir);
                    }
                    printf("\n");
                }
                else{
                    printf("Secilen sehirler arasi baglanti bulunmuyor!");
                }
            }
        }
    }
}
