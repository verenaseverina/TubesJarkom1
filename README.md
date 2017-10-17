1. Petunjuk Pengerjaan Program
- Buka folder TubesJarkom1 pada terminal
- Ketik "make" untuk menjalankan program

2. Cara Kerja Sliding Window
Sliding window yang diimplementasikan pada program ini dilakukan dengan menggunakan tiga thread. Thread yang pertama digunakan untuk penerima (receiver), thread yang kedua digunakan untuk pengirim (sender), dan thread yang ketiga digunakan untuk timeout. Jika ada paket yang belum diterima ACKnya pada satu window maka akan di-lock agar dapat menunggu sampai ACK diterima baru dilanjutkan lagi. Ukuran sliding window dapat "mengecil" jika buffer yang hendak digunakan belum direlease.

3. Pembagian Tugas
- 13515011 mengerjakan socket, file, ack.
- 13515047 mengerjakan sender.
- 13515107 mengerjakan receiver dan packet.

4. Jawaban dari Pertanyaan
-  Apa yang terjadi jika advertised window yang dikirim bernilai 0? Apa cara untuk menangani hal tersebut?
Jawaban: Sender menjadi tidak dapat mengirim paket lagi. Setelah timeout, sender akan retry dan mengirim paket lagi. Jika masih tidak bisa, sender akan menunggu lagi sampai advertised window size lebih besar dari 0.

- Sebutkan field data yang terdapat TCP Header serta ukurannya, ilustrasikan, dan jelaskan kegunaan dari masing-masing field data tersebut!
Jawaban: Field data yang terdapat TCP Header adalah SOH (1 byte), Sequence Number (4 byte), STX (1 byte), ETX (1 byte), ACK (1 byte), Next Sequence Number (4 byte), dan Advertised Window Size (1 byte). SOH atau Start of Header berfungsi sebagai awal dari sebuah paket yang juga merupakan awal dari header paket. Sequence Number berguna untuk menandakan urutan paket dan penempatannya agar tidak terjadi duplikasi. STX berfungsi sebagai awal dari teks atau data, dan kebalikannya ETX berfungsi sebagai akhir dari teks atau data. ACK atau acknowledgment berfungsi untuk memberitahukan paket yang sudah sampai, Next Sequence Number berfungsi untuk menunjukan paket dengan Sequence Number mana yang harus dikirim selanjutnya, Advertised Window Size berfungsi untuk mengetahui ukuran dari window yang bisa menjadi buffer.
* tambahan: pada program diasumsikan jika window size lebih besar dari sama dengan 256 byte, maka dijadikan menjadi 256 byte sesuai dengan ukuran buffer.