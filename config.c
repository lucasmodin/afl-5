#include "config.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * Læser en fil med navnet 'filnavn' og returnerer en 'struct config_t *',
 * altså en pointer til en 'struct config_t' der indeholder en setting for
 * hver linje i config-filen.
 */
struct config_t *read_config(char *filename) {

    //hvis fil stien ikke findes
    if (filename == NULL) {
        fprintf(stderr, "Filename is empty\n");
        return NULL;
    }

    //hvis filen fejler at åbne, skal funktionen fejle
    FILE *fr = fopen(filename, "r");
    if (fr == NULL) {
        fprintf(stderr, "Could not open %s\n", filename);
        return NULL;
    }

    //config_t holder alle settings i en fil
    struct config_t *config = malloc(sizeof(struct config_t));
    if (!config) {
        return NULL; //safe guard
    }

    config->count = 0; //holder styr på hvor mange settings der er læst ind
    char buf1[256]; //stack allokeret buffer der holder en linje fra filen af gangen

    //vi læser filen linje for linje indtil filen slutter
    while (fgets(buf1, sizeof(buf1), fr) != NULL) {

        //vi laver en heap allokeret kopi af filen
        char *buf2 = malloc(strlen(buf1) + 1);
        strcpy(buf2, buf1);

        //vi benytter vores setting_converter metode til at konvertere linjen til en setting_t
        struct setting_t *setting = setting_converter(buf2);
        free (buf2); // vi kan frigøre linjekopien da vi har allokeret i setting_converter metoden
        if (setting) {
            config->lines[config->count] = setting; //vi gemmer på den aktuelle plads
            config->count = config->count + 1; //vi inkrementere count efter
        }
    }
    fclose(fr); //og vi lukker selvfølgelig filen til sidst

    return config;
}

struct setting_t *setting_converter(char *line) {
    // TODO: Allokér en struct setting_t (vha. malloc())
    struct setting_t *setting = malloc(sizeof(struct setting_t));
    if (!setting) {
        return NULL; //safe guard
    }

    int eq_index = -1; //

    for (int i = 0; line[i] != '\0'; i++) {
        if (line[i] == '=') {
            eq_index = i;
            break;
        }
    }

    if (eq_index == -1) {
        return NULL; //hvis der ikke er et '=' tegn, så skal vi returnere null;
    }

    line[eq_index] = '\0'; //vi splitter strengen ved '=' ved at sætte en nul-byte

    char *left = line;
    char *right = &line[eq_index + 1];

    setting->name = malloc(strlen(left) + 1);
    setting->value = malloc(strlen(right) + 1);

    if (!setting->name || !setting->value) {
        free(setting->name);
        free(setting->value);
        free(setting);
        return NULL; //endnu en safe guard hvis allokering fejler
    }

    strcpy(setting->name, left);
    strcpy(setting->value, right);

    // TODO: return den setting hvor felterne er sat
    return setting;
}

void print_setting(struct setting_t *setting) {
    // TODO: Print en enkelt setting's name og value
    printf("%s = %s\n", setting->name, setting->value);
}

void print_config(struct config_t *config) {
    for (int i = 0; i < config->count; i++) {
        print_setting(config->lines[i]);
    }
}

// Det kan godt være, `free_config()` ikke frigør alle allokeringer, når I har omskrevet det
// til at bruge `struct setting_t`'er uden at I også opdaterer `free_config()`. Det er ikke
// et krav til afleveringsopgaven at I gør det.
void free_config(struct config_t *config) {
    if (config == NULL) {
        return;
    }

    for (int i = 0; i < config->count; i++) {
        free(config->lines[i]->name);
        free(config->lines[i]->value);
        free(config->lines[i]);
    }
    free(config);
}