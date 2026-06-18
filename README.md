# Analisis Nilai Mahasiswa - Docker VPS

![Course](https://img.shields.io/badge/Mata%20Kuliah-Komputasi%20Paralel-blue)
![Language](https://img.shields.io/badge/Language-C-555555)
![Runtime](https://img.shields.io/badge/Runtime-Docker%20VPS-2496ED)
![Dataset](https://img.shields.io/badge/Dataset-100%20Data-success)

Repository ini berisi laporan dan source code tugas kelompok mata kuliah **Komputasi Paralel**. Program digunakan untuk membaca dataset nilai mahasiswa dari file CSV, lalu menghitung nilai tertinggi, nilai terendah, dan rata-rata kelas.

Pelaksanaan tugas menggunakan alur terbaru: tidak lagi menghubungkan node anggota satu per satu. Semua proses dijalankan melalui VPS dosen `kelasa@103.253.213.240`, lalu lingkungan node disimulasikan menggunakan Docker.

## Ringkasan Proyek

| Komponen | Keterangan |
| --- | --- |
| Mata Kuliah | Komputasi Paralel |
| Topik | Analisis data nilai mahasiswa |
| Bahasa | C |
| Lingkungan | VPS dosen dan Docker |
| Input | `data_nilai.csv` |
| Source | `mpi_nilai2.c` |
| Hostfile | `hosts.txt` |
| Output | Total data, nilai maksimum, nilai minimum, rata-rata |

## Anggota Kelompok

| No | NIM | Nama |
| --- | --- | --- |
| 1 | 24106050082 | Muhammad Ridhwan Hadayana Malik |
| 2 | 24106050074 | Hamdan Rahmadin Akbar |
| 3 | 24106050054 | Ahmad Faiq Zada |
| 4 | 24106050078 | Muhammad Ilham Hanafi |
| 5 | 24106050014 | Mia Amelia |
| 6 | 24106050015 | Zahrotul Ngaliyah |

## Arsitektur Lingkungan

Alur awal pada `4_tugas.pdf` meminta node dihubungkan antaranggota. Pada pelaksanaan terbaru, cluster dijalankan di dalam Docker pada VPS dosen sehingga seluruh anggota menggunakan lingkungan yang sama.

```text
Laptop Anggota
      |
      | ssh
      v
VPS Dosen: kelasa@103.253.213.240
      |
      | Docker
      v
+------------------+     +-------------------+     +-------------------+
| kelas-a-master   |     | kelas-a-worker1   |     | kelas-a-worker2   |
| slots=2          |     | slots=2           |     | slots=2           |
+------------------+     +-------------------+     +-------------------+
```

Isi `hosts.txt`:

```text
kelas-a-master slots=2
kelas-a-worker1 slots=2
kelas-a-worker2 slots=2
```

## Struktur File

```text
.
|-- 4_tugas.pdf
|-- LAPORAN_TUGAS_MPI_DOCKER.html
|-- README.md
|-- data_nilai.csv
|-- hosts.txt
|-- mpi_nilai2
`-- mpi_nilai2.c
```

## Dataset

File `data_nilai.csv` memiliki dua kolom:

| Kolom | Deskripsi |
| --- | --- |
| `NIM` | Nomor induk mahasiswa |
| `Nilai` | Nilai mahasiswa dengan rentang 0 sampai 100 |

Contoh isi data:

```csv
NIM,Nilai
220101001,85.5
220101002,90.0
220101003,67.2
```

Dataset yang digunakan berisi 101 baris, yaitu 1 header dan 100 data nilai mahasiswa.

## Penjelasan Program

Program utama berada pada `mpi_nilai2.c`.

Tahapan program:

1. Membuka file `data_nilai.csv`.
2. Melewati baris header.
3. Membaca kolom `Nilai` pada setiap baris.
4. Memvalidasi nilai agar tetap berada pada rentang 0 sampai 100.
5. Menghitung nilai tertinggi, nilai terendah, dan total nilai.
6. Menghitung rata-rata kelas.
7. Menampilkan hasil akhir ke terminal.

Bagian konfigurasi file input:

```c
#define DEFAULT_INPUT_FILE "data_nilai.csv"
#define INITIAL_CAPACITY 128
#define LINE_SIZE 256
```

Bagian perhitungan nilai:

```c
for (int i = 0; i < total_count; i++) {
    double score = all_scores[i];

    if (score > max_score) {
        max_score = score;
    }
    if (score < min_score) {
        min_score = score;
    }
    sum_score += score;
}

double average = sum_score / (double)total_count;
```

## Cara Menjalankan

Masuk ke VPS dosen:

```bash
ssh kelasa@103.253.213.240
```

Cek container yang berjalan:

```bash
docker ps
```

Masuk ke container utama:

```bash
docker exec -it kelas-a-master bash
```

Compile program:

```bash
gcc mpi_nilai2.c -o mpi_nilai2
```

Jalankan program:

```bash
./mpi_nilai2
```

Jika ingin mengikuti format eksekusi dengan hostfile pada lingkungan Docker:

```bash
mpirun --allow-run-as-root --hostfile hosts.txt -np 6 ./mpi_nilai2
```

## Output Program

```text
===========================================
HASIL ANALISIS NILAI MAHASISWA (SEKUENSIAL)
===========================================
Total Data Diproses : 100 baris
Nilai Tertinggi     : 98.50
Nilai Terendah      : 62.40
Rata-rata Kelas     : 80.94
===========================================
```

## Analisis Hasil

| Metrik | Hasil |
| --- | ---: |
| Total data valid | 100 baris |
| Nilai tertinggi | 98.50 |
| Nilai terendah | 62.40 |
| Rata-rata kelas | 80.94 |

Hasil tersebut menunjukkan bahwa program berhasil membaca seluruh data valid dari CSV dan menghitung statistik dasar nilai mahasiswa sesuai format keluaran tugas.

## Catatan Implementasi

- Alur pengerjaan memakai VPS dosen dan Docker, bukan koneksi langsung antar laptop anggota.
- `hosts.txt` memakai hostname container Docker.
- `data_nilai.csv` dan `mpi_nilai2.c` harus berada pada direktori kerja yang sama saat program dijalankan.
- File laporan utama tersedia pada `LAPORAN_TUGAS_MPI_DOCKER.html`.

## Laporan

Laporan lengkap dalam format HTML dapat dibuka melalui:

```text
LAPORAN_TUGAS_MPI_DOCKER.html
```

File tersebut berisi penjelasan tugas, anggota kelompok, konfigurasi lingkungan, penjelasan script, command eksekusi, output, dan analisis hasil.
