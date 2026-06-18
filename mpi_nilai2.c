#include <float.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DEFAULT_INPUT_FILE "data_nilai.csv"
#define INITIAL_CAPACITY 128
#define LINE_SIZE 256

// Fungsi pembacaan CSV (Sama persis seperti versi MPI kamu)
static int read_scores(const char *filename, double **scores_out)
{
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        return -1;
    }

    int capacity = INITIAL_CAPACITY;
    int count = 0;
    double *scores = malloc((size_t)capacity * sizeof(double));
    char line[LINE_SIZE];

    if (scores == NULL) {
        fclose(file);
        return -1;
    }

    // Lewati baris pertama (header)
    fgets(line, sizeof(line), file);

    while (fgets(line, sizeof(line), file) != NULL) {
        char *comma = strchr(line, ',');
        char *endptr = NULL;
        double score;

        if (comma == NULL) {
            continue;
        }

        score = strtod(comma + 1, &endptr);
        if (endptr == comma + 1) {
            continue;
        }

        if (score < 0.0 || score > 100.0) {
            continue;
        }

        if (count == capacity) {
            int new_capacity = capacity * 2;
            double *resized = realloc(scores, (size_t)new_capacity * sizeof(double));
            if (resized == NULL) {
                free(scores);
                fclose(file);
                return -1;
            }
            scores = resized;
            capacity = new_capacity;
        }

        scores[count] = score;
        count++;
    }

    fclose(file);
    *scores_out = scores;
    return count;
}

int main(int argc, char **argv)
{
    int total_count = 0;
    double *all_scores = NULL;
    double min_score = DBL_MAX;
    double max_score = -DBL_MAX;
    double sum_score = 0.0;
    const char *input_file = DEFAULT_INPUT_FILE;

    // Ambil nama file dari argumen jika ada
    if (argc > 1) {
        input_file = argv[1];
    }

    // Membaca file CSV
    total_count = read_scores(input_file, &all_scores);
    if (total_count <= 0) {
        fprintf(stderr, "Gagal membaca data nilai dari file CSV.\n");
        return 1;
    }

    // Kalkulasi data secara langsung (tanpa Scatterv atau Reduce)
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

    // Menghitung rata-rata
    double average = sum_score / (double)total_count;

    // Menampilkan output hasil analisis
    printf("===========================================\n");
    printf("HASIL ANALISIS NILAI MAHASISWA (SEKUENSIAL)\n");
    printf("===========================================\n");
    printf("Total Data Diproses : %d baris\n", total_count);
    printf("Nilai Tertinggi     : %.2f\n", max_score);
    printf("Nilai Terendah      : %.2f\n", min_score);
    printf("Rata-rata Kelas     : %.2f\n", average);
    printf("===========================================\n");

    // Bebaskan memori
    free(all_scores);

    return 0;
}
