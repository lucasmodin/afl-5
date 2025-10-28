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
    // Hvis 'filename' er NULL, skal funktionen fejle.
    if (filename == NULL) {
        fprintf(stderr, "Filename is empty\n");
        return NULL;
    }

    // Hvis filen fejler at åbne, skal funktionen fejle.
    FILE *fr = fopen(filename, "r");
    if (fr == NULL) {
        fprintf(stderr, "Could not open %s\n", filename);
        return NULL;
    }

    // Der allokeres plads til `struct config_t`-værdien
    struct config_t *config = malloc(sizeof(struct config_t));

    // Der er blevet indlæst 0 settings i config'en
    config->count = 0;

    // Der stak-allokeres plads til at indlæse en enkelt linje. Det er okay med
    // en stak-allokering her, fordi den er midlertidig: vi skal ikke bruge bufferen
    // når vi først har indlæst hele konfigurationen. Så den er midlertidig inde i
    // funktionen.
    char buf1[256];

    // - `fgets()` returnerer NULL når vi er nået bunden af filen
    // - Der læses højest `sizeof(buf1)` bytes ind i buf1
    // - `fgets()` husker at gøre plads til NUL-byte'en!
    while (fgets(buf1, sizeof(buf1), fr) != NULL) {
        // Der heap-allokeres plads til den enkelte linje. Det er nødvendigt
        // med heap-allokering fordi den ellers vil gå tabt når read_config()
        // returnerer. Og `malloc()` tager ikke højde for NUL-byt'en. :(
        char *buf2 = malloc(strlen(buf1) + 1);

        // `buf1` kopieres ind i `buf2`: Fra den midlertidige stak-allokerede
        // buffer buf1, som genbruges for hver linje, til de dedikerede heap-
        // allokerede buffere buf2, som der er én af for hver linje i filen.
        strcpy(buf2, buf1);

        // Efter `buf2` er allokeret og config-linjen er kopieret ind, skal
        // pointeren gemmes i vores datastruktur, så den ikke går tabt. Den
        // gemmes på plads `config->count` som er seneste ubrugte plads.
        config->lines[config->count] = buf2; // brug setting_converter()

        // `count` forøges så næste config-linjes buffer gemmes på næste plads.
        config->count += 1;
    }

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
        struct setting_t *setting = setting_converter(config->lines[i]);
        print_setting(setting);
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
        free(config->lines[i]);
    }

    free(config);
}