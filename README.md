# Tugas Besar 1 IF3130 Jaringan Komputer - Sliding Window
## Kelompok 10 - Verrerro


### 1. Petunjuk Pengerjaan Program
- Lakukan kompilasi terlebih dahulu dengan menjalankan __make__  pada terminal
- Jalankan executable __sendfile__ dan __recvfile__ dengan parameter di bawah ini. Pastikan untuk menjalankan __recvfile__ terlebih dahulu sebelum menjalakan __sendfile__.

  - __./recvfile__ {filename} {windowsize} {buffersize} {port} > {recvfile_log}
  - __./sendfile__ {filename} {windowsize} {buffersize} {destination_ip} {destination_port} > {sendfile_log}
 

### 2. Cara Kerja Sliding Window
Cara kerja dari sliding window di program ini adalah :

- Sender :

1. Sender akan mengirimkan paket sejumlah window size.
2. Setelah selesai, sender akan menunggu ack dari receiver selama *timeout* belum terjadi.
3. Jika *timeout* sudah terjadi, maka cek apakah window size = 0, yang menandakan pengiriman sudah selesai. Jika sudah selesai, keluar dari program. Jika tidak, kembali ke poin 1 dengan sequence number = LAR + 1.

- Receiver
1. Receiver akan menunggu paket dari sender.
2. Jika recv buffer kosong, maka data pada paket akan dimasukkan ke file buffer jika sequence number paket sesuai dengan next sequence number yang dikirimkan pada ack sebelumnya, dan dimasukkan ke recv buffer jika sequence number paket tidak sesuai dengan next sequence number yang dikirimkan pada ack sebelumnya.
3. Jika recv buffer tidak kosong, maka data pada paket akan dimasukkan ke recv buffer. Cari semua data terurut dari awal recv buffer. Jika ada, pindahkan semua data tersebut ke file buffer dan geser sisa data pada recv buffer ke depan.
4. Increment nilai LAF dan LFR sesuai dengan jumlah data yang masuk ke file buffer. Jika jumlah data di file buffer < file size, kembali ke poin 1.

### 3. Pembagian Tugas
- 13515011 mengerjakan socket, file, ack.
- 13515047 mengerjakan sender.
- 13515107 mengerjakan receiver dan packet.

### 4. Jawaban dari Pertanyaan

#### Apa yang terjadi jika advertised window yang dikirim bernilai 0? Apa cara untuk menangani hal tersebut?

__Jawaban:__ 
Sender akan berhenti mengirimkan paket. Setelah *timeout*, sender akan mengirim ulang paket. Jika advertised window size = 0, sender akan menunggu lagi sampai *timeout*, sebelum mencoba mengirimkan paket lagi. Jika advertised window size > 0, maka sender akan mengirimkan paket seperti biasa.

#### Sebutkan field data yang terdapat TCP Header serta ukurannya, ilustrasikan, dan jelaskan kegunaan dari masing-masing field data tersebut!
__Jawaban:__ 
Field data yang terdapat TCP Header adalah SOH (1 byte), Sequence Number (4 byte), STX (1 byte), ETX (1 byte), ACK (1 byte), Next Sequence Number (4 byte), dan Advertised Window Size (1 byte). SOH atau Start of Header berfungsi sebagai awal dari sebuah paket yang juga merupakan awal dari header paket. Sequence Number berguna untuk menandakan urutan paket dan penempatannya agar tidak terjadi duplikasi. STX berfungsi sebagai awal dari teks atau data, dan kebalikannya ETX berfungsi sebagai akhir dari teks atau data. ACK atau acknowledgment berfungsi untuk memberitahukan paket yang sudah sampai, Next Sequence Number berfungsi untuk menunjukan paket dengan Sequence Number mana yang harus dikirim selanjutnya, Advertised Window Size berfungsi untuk menginformasikan sender berapa sisa buffer pada receiver yang dapat menampung paket dari sender.

*Asumsi* : Jika window size lebih besar dari sama dengan 256 bytes, maka window size di-set menjadi 256 bytes sesuai dengan ukuran buffer.
