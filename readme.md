# Digital Library Tugas Akhir ( Thesis )
## Sarjana Terapan ( S.Tr ) Teknik Telekomunikasi
## Politeknik Negeri Semarang ( Polines )
( Mixed language Bahasa & English )


## Dikembangkan oleh Benny Hartanto Setiyadi
### Bermitra dengan :
- Gunardi sebagai Penanggung Jawab dari Rumah Maggot Kabupaten Semarang.
### Dibawah bimbingan :
- Ari Sriyanto N., S.T., M.T., M.Sc., ( Dosen Pembimbing I ).
- Sindung H.W.S., B.S.E.E, M.Eng.Sc., ( Dosen Pembimbing II ).

## Judul Tugas Akhir
  ### " Sistem Pengendalian dan Monitoring Produksi Telur Lalat Black Soldier Fly "
  ### Berkas PDF Tugas Akhir
    (Silahkan klik tautan ini)

## Abstrak
(in-progress)
## Preview
(in-progress)
## Fitur - fitur
- Mengendalikan kondisi _water pump_ ( LOW | HIGH ).
- Mengendalikan kondisi lampu _fertilizer_ ( LOW | HIGH ).
- Kendali _hybrid_ menggunakan perintah bot telegram maupun _Manual GPIO Switch_.
- Kendali kondisi otomatis melalui perintah _/schedule_ melalui bot telegram.
- _Monitoring_ suhu dan kelembapan melalui bot telegram.
- _Monitoring_ intensitas cahaya melalui bot telegram.
- Sistem dapat bekerja di saat internet putus sekalipun dengan syarat pada kondisi awal atau setelah reset pernah terhubung ke internet satu kali.

## Bugs
- Saat koneksi internet _unstable_, terkadang akan memunculkan _duplicate messages_.
- ~~Saat proses _booting_, relay akan menyala di awal hingga program selesai dimuat (khusus untuk konfigurasi relay _Normally Close_ atau _NC_).~~
- Jika pesan balasan dari bot telegram lambat atau tidak muncul sama sekali perhatikan pada konfigurasi antenna ESP32.
- Tidak bisa melayani request dalam jumlah banyak kecuali dengan cara bergantian.
- Perintah /schedule pada bot telegram kemungkinan akan terganggu jika koneksi WiFi terputus dikarenakan menggunakan sinkronisasi waktu _NTP_.
- ~~Masalah perintah _/switch_ pada bot telegram.~~
- Masalah perintah /schedule dimana aktuator tidak berhenti setelah perintah stop dijalankan.

(in-progress)
## Spesifikasi
- _Panel box dimensions_ (cm, cm, cm, cm).
- _Single_ 220VAC _power input splitted to_ ESP32, lampu _fertilizer_ dan _water pump_.
- 20x4 _LCD Display_.
- DHT11 _sensor_.
- BH1750 _sensor_.
- 2.4 GHz WiFi _antenna_ komunikasi.
- _Water pump_ 130PSi 12VDC dengan adapter AC.
- Lampu _fertilizer_ warna _bright purple_ dengan _female_ AC _jack_ 3 meter.
- _Manual GPIO Switch_ (_enable_, lampu _fertilizer_, _water pump_).
- _Reset Switch_

## Requirements and How to Set-Up
Koneksi internet yang lancar untuk terhubung satu kali, komponen - komponen elektronika yang sesuai dengan _wiring diagram_ dan untuk harga komponen yang dimaksud tersedia pada _Bill of Material_.

**Saya anggap siapapun yang akan mencoba mempraktikkan atau berniat menggunakan kode ini sepenuhnya sadar dan memahami tentang pengkabelan jalur kabel seperti yang tertera pada _Wiring Diagram_, cara menggunakan Vscode beserta dengan ekstensi PlatformIO. Jika sudah, ikutilah langkah singkat dibawah ini.**

(in-progress)
## Wiring Diagram
(in-progress)
## Bill of Material ( BoM )
(in-progress)