# SoalShift_modul4_F01

1. Putri Endah Puspitasari (05111740000039)<br>
2. Garda Negara Wisnumurti (05111740000153)

Suatu ketika King of YouTube ASEAN, Atta Halilintar, mengunjungi laboratorium AJK karena urusan yang benar-benar genting. 
Prestasi dan ketenarannya ternyata menarik perhatian hacker yang ingin mencuri video-video berkualitas milik Atta, bahkan ingin mencuri password akun YouTube miliknya. 
Menurut intel Atta, pemimpin hacker yang berusaha menyerangnya adalah :

Hacker tersebut menyusup ke dalam file system yang ada di dalam PC Atta dengan mudah karena ternyata file system tersebut sama sekali tidak memiliki sistem pengamanan. Maka dari itu maksud Atta ke laboratorium AJK karena dia ingin kalian untuk mengembangkan file system yang aman.

Atta menginginkan nama file system yang akan kalian buat adalah “__AFSHiaAP__”, yang nama kepanjangannya adalah AHHA’s File System: Hierarchical and Advanced Protection. Berikut ini kebutuhan yang harus kalian penuhi:

## Soal 1
Semua nama file dan folder harus terenkripsi
Enkripsi yang Atta inginkan sangat sederhana, yaitu Caesar cipher. Namun, Kusuma mengatakan enkripsi tersebut sangat mudah dipecahkan. Dia menyarankan untuk character list diekspansi,tidak hanya alfabet, dan diacak. Berikut character list yang dipakai :

```qE1~ YMUR2"`hNIdPzi%^t@(Ao:=CQ,nx4S[7mHFye#aT6+v)DfKL$r?bkOGB>}!9_wV']jcp5JZ&Xl|\8s;g<{3.u*W-0```

Misalkan ada file bernama “halo” di dalam folder “INI_FOLDER”, dan key yang dipakai adalah 17, maka:

__“INI_FOLDER/halo”__ saat belum di-mount maka akan bernama __“n,nsbZ]wio/QBE#”__, saat telah di-mount maka akan otomatis terdekripsi kembali menjadi __“INI_FOLDER/halo”__ .
Perhatian: Karakter ‘/’ adalah karakter ilegal dalam penamaan file atau folder dalam *NIX, maka dari itu dapat diabaikan

Penjelasan :

Dari soal kita diminta untuk meng-enkrip kan suatu nama folder atau file dengan Caesar cipher, semua folder dan file yang dibuat akan ter enkripsi sebelum program di mount, setelah program di mount maka akan ter dekripsi. Dalam penggunaan Caesar cipher menggunakan key yaitu kunci yang digunakan untuk memudahkan proses enkripsi dan dekripsi. Berdasarkan enkripsi dekripsi Caesar cipher kita dapatkan rumus sebagai berikut :

* Enkripsi : (indeks + key) mod (jml_list)

* Dekripsi : (indeks + (jml_list - key)) mod (jml_key)

Indeks merupakan huruf yang di inputkan itu indeks ke berapa di list yang sudah di sediakan.
Sehingga kita dapatkan suatu source code untuk Enkripsi nya yaitu :

* Enkripsi :

```javascript
void enkripsi(char* kata)
{
    char dasar[100] = "qE1~ YMUR2\"`hNIdPzi%^t@(Ao:=CQ,nx4S[7mHFye#aT6+v)DfKL$r?bkOGB>}!9_wV']jcp5JZ&Xl|\\8s;g<{3.u*W-0";
    for(int i=0; i<strlen(kata); i++){
        for(int j = 0 ; j<strlen(dasar); j++){
            if(kata[i] == dasar[j]){
                int indeks_baru = (j+17) % 94;
                kata[i] = dasar[indeks_baru];
                break;
            }
        }
    }
}
```
Dari source code diatas kita hitung dulu panjang kata yang akan di inputkan dengan ```  for(int i=0; i<strlen(kata); i++){ ``` jika i kurang dari panjang inputan kata dan j kurang dari panjang list dasar maka 

```if(kata[i] == dasar[j]){``` jika kata ke i sama dengan dasar ke j maka menggunakan rumus enkripsi yaitu 

``` int indeks_baru = (j+17) % 94;``` indeks baru sama dengan j + 17 sebagai key di mod 94 yaitu banyaknya karakter dasar.

``` kata[i] = dasar[indeks_baru];``` maka kata i berubah menjadi dasar ke indeks yang baru, sama seperti penjelasan saya sebelumnya.

* Dekripsi :

```javascript
void dekripsi(char* kata)
{
    char dasar[100] = "qE1~ YMUR2\"`hNIdPzi%^t@(Ao:=CQ,nx4S[7mHFye#aT6+v)DfKL$r?bkOGB>}!9_wV']jcp5JZ&Xl|\\8s;g<{3.u*W-0";
    for(int i=0; i<strlen(kata); i++){
        for(int j = 0 ; j<strlen(dasar); j++){
            if(kata[i] == dasar[j]){
                int indeks_baru = (j+(94-17)) % 94;
                kata[i] = dasar[indeks_baru];
                break;
            }
        }
    }
}
```
Sama seperti halnya dengan enkripsi, dekripsi pun juga sama hanya saja berbeda pada rumus nya saja.

Dari source code diatas kita hitung dulu panjang kata yang akan di inputkan dengan ```  for(int i=0; i<strlen(kata); i++){ ``` jika i kurang dari panjang inputan kata dan j kurang dari panjang list dasar maka 

```if(kata[i] == dasar[j]){``` jika kata ke i sama dengan dasar ke j maka menggunakan rumus enkripsi yaitu 

``` int indeks_baru = (j+(94-17)) % 94;``` indeks baru sama dengan j + 94 dikurang 17 sebagai key kemudian di mod 94 yaitu banyaknya karakter dasar.

``` kata[i] = dasar[indeks_baru];``` maka kata i berubah menjadi dasar ke indeks yang baru, sama seperti penjelasan saya sebelumnya.

Kemudian pada setiap fungsi di tambahkan source code berikut 

```javascript
char fpath[1000];
	char sementara[1000];
    sprintf(sementara,"%s",path);

    enkripsi(sementara);
	sprintf(fpath, "%s%s",dirpath,sementara);
```

``` sprintf(sementara,"%s",path);``` simpan path pada variabel sementara.

```enkripsi(sementara);``` agar yang terenkripsi nantinya adalah folder dan file yang ada di sementara.

```sprintf(fpath, "%s%s",dirpath,sementara);``` kemudian path keseluruhannya ditambah dengan path sememntara di simpan di variabel bernama fpath.

## Soal 2
Semua file video yang tersimpan secara terpecah-pecah (splitted) harus secara otomatis tergabung (joined) dan diletakkan dalam folder “Videos”
Urutan operasi dari kebutuhan ini adalah:

a. Tepat saat sebelum file system di-mount

   i.   Secara otomatis folder “Videos” terbuat di root directory file system
   
   ii.  Misal ada sekumpulan file pecahan video bernama “computer.mkv.000”,         “computer.mkv.001”, “computer.mkv.002”, dst. Maka         secara otomatis file pecahan tersebut akan di-join menjadi file video “computer.mkv”
Untuk mempermudah kalian, dipastikan hanya video file saja yang terpecah menjadi beberapa file. File pecahan tersebut dijamin terletak di root folder fuse

  iii.  Karena mungkin file video sangat banyak sehingga mungkin saja saat menggabungkan file video, file system akan membutuhkan waktu yang lama untuk sukses ter-mount. Maka pastikan saat akan menggabungkan file pecahan video, file system akan membuat __1 thread/proses(fork) baru__ yang dikhususkan untuk menggabungkan file video tersebut

   iv.   Pindahkan seluruh file video yang sudah ter-join ke dalam folder “Videos”
   
   v.    Jangan tampilkan file pecahan di direktori manapun
   
b. Tepat saat file system akan di-unmount

  i.    Hapus semua file video yang berada di folder “Videos”, tapi jangan hapus file pecahan yang terdapat di root directory file system
  
  ii.   Hapus folder “Videos”

## Soal 3
Sebelum diterapkannya file system ini, Atta pernah diserang oleh hacker LAPTOP_RUSAK yang menanamkan user bernama “chipset” dan “ic_controller” serta group “rusak” yang tidak bisa dihapus. Karena paranoid, Atta menerapkan aturan pada file system ini untuk menghapus “file bahaya” yang memiliki spesifikasi:

  - Owner Name 	: ‘chipset’ atau ‘ic_controller’
  - Group Name	: ‘rusak’
  - Tidak dapat dibaca
  
Jika ditemukan file dengan spesifikasi tersebut ketika membuka direktori, Atta akan menyimpan nama file, group ID, owner ID, dan waktu terakhir diakses dalam file “filemiris.txt” (format waktu bebas, namun harus memiliki jam menit detik dan tanggal) lalu menghapus “file bahaya” tersebut untuk mencegah serangan lanjutan dari LAPTOP_RUSAK.

## Soal 4
Pada folder __YOUTUBER,__ setiap membuat folder permission foldernya akan otomatis menjadi 750. Juga ketika membuat file permissionnya akan otomatis menjadi 640 dan ekstensi filenya akan bertambah __“.iz1”__. File berekstensi “.iz1” tidak bisa diubah permissionnya dan memunculkan error bertuliskan “File ekstensi iz1 tidak boleh diubah permissionnya.”

## Soal 5
Ketika mengedit suatu file dan melakukan save, maka akan terbuat folder baru bernama __Backup__ kemudian hasil dari save tersebut akan disimpan pada backup dengan nama __namafile_[timestamp].ekstensi.__ Dan ketika file asli dihapus, maka akan dibuat folder bernama __RecycleBin__, kemudian file yang dihapus beserta semua backup dari file yang dihapus tersebut (jika ada) di zip dengan nama __namafile_deleted_[timestamp].zip__ dan ditaruh kedalam folder RecycleBin (file asli dan backup terhapus). Dengan format __[timestamp]__ adalah __yyyy-MM-dd_HH:mm:ss__

__Catatan:__ 

* Semua nama file dan folder yang terdapat pada soal harus memenuhi syarat soal no. 1 (terenkripsi saat belum di-mount dan terdekripsi saat di-mount)

* Tidak diperkenankan menggunakan __system()__ dan __exec*()__, kecuali ada pengecualian di butir soal.

* Untuk soal nomor 3 diharapkan secara __manual__ membuat __user__ dan __group__-nya. Pengecekan file dilakukan setiap membuka direktori.

* Untuk soal nomor 4 dan 5 diperbolehkan menggunakan __exec*().__

* __Pengerjaan hanya dilakukan dalam 1 file program C dengan format nama AFSHiaAP_[Kelompok].c .__

* File System __AFSHiaAP__ mengarah ke folder __/home/[user]/shift4.__


