1. Petunjuk Pengerjaan Program
- Buka folder TubesJarkom1 pada terminal
- Ketik "make" untuk menjalankan program

2. Cara Kerja Sliding Window
Sliding window pada implementasi ini adalah dengan cara membuat window yang sesuai dengan ukuran yang didefinisikan pada input dari keyboard. Kemudian menggunakan thread agar beberapa paket dapat dikirimkan pada saat yang bersamaan. Jika ada paket yang belum diterima ACKnya pada satu window maka akan di-lock agar dapat menunggu sampai ACK diterima baru dilanjutkan lagi. Ukuran sliding window dapat "mengecil" jika buffer yang hendak digunakan belum direlease.



3. Pembagian Tugas
- 13515011 mengerjakan socket, file, ack.
- 13515047 mengerjakan sender.
- 13515107 mengerjakan receiver dan packet.

4. Jawaban dari Pertanyaan
-  Apa yang terjadi jika advertised window yang dikirim bernilai 0? Apa cara untuk menangani hal tersebut?
Jawaban: Sender menjadi tidak dapat mengirim paket lagi. Setelah timeout, sender akan retry dan mengirim paket lagi. Jika masih tidak bisa, sender akan menunggu lagi sampai advertised window size lebih besar dari 0.


- Sebutkan field data yang terdapat TCP Header serta ukurannya, ilustrasikan, dan jelaskan kegunaan dari masing-masing field data tersebut!
Jawaban: 