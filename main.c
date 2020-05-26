/*
** EPITECH PROJECT, 2019
** 103cipher
** File description:
** Let's do some calculatations ! Nevermind, that's not my part.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void disp_hflag(char **av)
{
    printf("USAGE\n    ");
    printf("%s message key flag\n\n", av[0]);
    printf("DESCRIPTION\n");
    printf("    message     a message, made of ASCII characters\n");
    printf("    key         the encryption key, made of ASCII characters\n");
    printf("    flag        0 for the message to be encrypted, 1 to be decrypted\n");
}

int deal_error(int ac, char **av)
{
    if (ac != 4) {
        printf("Wrongs numbers of arguments -- Error\n");
        return (84);
    }
    if ('0' != av[3][0] && '1' != av[3][0]) {
        printf("The third argument isn't correct -- Error\n");
        return (84);
    }
    return (1);
}

int get_nb_colums(char *src)
{
    int square = 0;

    for (int i = 1 ; strlen(src) > i * i ; ++i)
        square = i;
    ++square;
    return (square);
}

int get_nb_lines(char *src, int size)
{
    int square = 0;

    for (int i = 1 ; strlen(src) > i * size ; ++i)
        square = i;
    ++square;
    return (square);
}

float **key_into_numbers(char *src, int *size, int *cols)
{
    int l = 0;
    int c = 0;
    int lenght = strlen(src);
    if (*size == 0)
        *size = get_nb_colums(src);
    if (*cols != 0)
        *size = get_nb_lines(src, *size);
    float **tab = malloc(sizeof(float *) * (*size + 1));
    float *key_nb = malloc(sizeof(float) * lenght);

    if (tab == NULL)
        return (NULL);
    tab[*size] = NULL;
    if (*cols == 0)
        *cols = *size;
    for (int i = 0; i < *size; ++i) {
        tab[i] = malloc(sizeof(float) * *cols);
        if (tab[i] == NULL)
            return (NULL);
    }
    if (key_nb == NULL)
        return (NULL);
    for (int i = 0; src[i]; ++i)
        key_nb[i] = src[i];
    for (int i = 0 ; i < lenght ; ++i) {
        tab[l][c] = key_nb[i];
        ++c;
        if (c == *cols) {
            c = 0;
            ++l;
        }
    }
    free(key_nb);
    return (tab);
}

void disp_key(float **tab, int size, int mode)
{
    printf("Key matrix:\n");
    if (mode == 0) {
        for (int j = 0 ; tab[j] ; j++) {
            for (int i = 0 ; i < size ; i++) {
                printf("%.f", tab[j][i]);
                if (i != size - 1)
                    printf("    ");
            }
            printf("\n");
        }
    } else {
        for (int j = 0 ; tab[j] ; j++) {
            for (int i = 0 ; i < size ; i++) {
                if (tab[j][i] == 0)
                    printf("%.1f", tab[j][i]);
                else
                    printf("%.3f", tab[j][i]);
                if (i != size - 1)
                    printf("    ");
            }
            printf("\n");
        }
    }
    printf("\n");
}

void encrypted_mess(float **tab, int cols)
{
    printf("Encrypted message:\n");
    for (int i = 0 ; tab[i] ; i++) {
        for (int j = 0 ; j < cols ; j++) {
            printf("%.f", tab[i][j]);
            if (!(tab[i + 1] == NULL && j == cols - 1))
                printf(" ");
        }
    }
    printf("\n");
}

float **matrix_mul(float **tab1, float **tab2, int size1, int size2)
{
    float **tab = malloc(sizeof(float *) * (size1 + 1));
    float result = 0;

    for (int i = 0 ; i < size1 ; i++)
        tab[i] = malloc(sizeof(float) * (size2 + 1));
    tab[size1] = NULL;
    for (int i = 0 ; i < size1 ; i++) {
        for (int j = 0 ; j < size2 ; j++) {
            for (int k = 0 ; k < size2 ; k++)
                result += tab1[i][k] * tab2[k][j];
            tab[i][j] = result;
            result = 0;
        }
    }
    for (int i = 0 ; tab1[i] ; i++)
        free(tab1[i]);
    free(tab1);
    for (int i = 0 ; tab2[i] ; i++)
        free(tab2[i]);
    free(tab2);
    return (tab);
}

void *encrypt(char **av)
{
    float **tab;
    float **tab1;
    float **tab2;
    int size1;
    int size2 = 0;
    int cols = 0;

    tab2 = key_into_numbers(av[2], &size2, &cols);
    if (tab2 == NULL)
        return (NULL);
    disp_key(tab2, size2, 0);
    size1 = size2;
    tab1 = key_into_numbers(av[1], &size1, &cols);
    if (tab1 == NULL)
        return (NULL);
    tab = matrix_mul(tab1, tab2, size1, size2);
    encrypted_mess(tab, cols);
    for (int i = 0 ; tab[i] ; i++)
        free(tab[i]);
    free(tab);
}

float **inv_matrix(float **tab1, int size)
{
    if (size > 2)
        return (NULL);
    float **tab = malloc(sizeof(float *) * (size + 1));
    float div;
    if (size == 2)
        div = tab1[1][1] * tab1[0][0] - tab1[0][1] * tab1[1][0];
    tab[size] = NULL;
    for (int i = 0 ; i < size ; i++)
        tab[i] = malloc(sizeof(float) * size);
    if (size == 2) {
        tab[0][0] = tab1[1][1] / div;
        tab[1][0] = tab1[1][0] / -div;
        tab[1][1] = tab1[0][0] / div;
        tab[0][1] = tab1[0][1] / -div;
    } else if (size == 1)
        tab[0][0] = 1 / tab1[0][0];
    for (int i = 0 ; tab1[i] ; i++)
        free(tab1[i]);
    free(tab1);
    return (tab);
}

char *matrix_to_str(float **tab, int size1, int size2)
{
    char *str = malloc(sizeof(char) * (size1 * size2 + 1));
    int j = 0;
    int k = 0;

    for (int i = 0 ; i < size1 * size2 ; i++) {
        str[i] = tab[j][k];
        k++;
        if (k == size2) {
            k = 0;
            j++;
        }
    }
    str[size1 * size2] = '\0';
    return (str);
}

int get_line_decript(char *src, int elem, int size)
{
    int square = 0;

    for (int i = 1 ; elem > i * size ; ++i)
        square = i;
    ++square;
    return (square);
}

char **get_each_elem(char *src, int elem)
{
    int elem_size = 0, l = 0, c = 0;
    char **str = malloc(sizeof(char *) * (elem + 1));
    int *len_elem = malloc(sizeof(int) * elem);

    if (len_elem == NULL)
        return (NULL);
    for (int i = 0; src[i]; ++i) {
        if (' ' == src[i]) {
            len_elem[l] = elem_size;
            elem_size = 0;
        }
        else
            ++elem_size;
    }
    if (str == NULL)
        return (NULL);
    for (int i = 0; elem > i; ++i) {
        str[i] = malloc(sizeof(char) * (len_elem[i] + 1));
        if (str[i] == NULL)
            return (NULL);
    }
    str[elem] = NULL;
    for (int i = 0; src[i]; ++i) {
        if (' ' == src[i]) {
            str[l][c] = '\0';
            ++l;
            c = 0;
        } else {
            str[l][c] = src[i];
            ++c;
        }
    }
    free(len_elem);
    return (str);
}

float **into_the_matrix(char *src, int *size, int cols)
{
    int elem = 0, l = 0, c = 0;
    float **tab;

    for (int i = 0; src[i]; ++i)
        if (' ' == src[i])
            ++elem;
    ++elem;
    *size = get_line_decript(src, elem, cols);
    tab = malloc(sizeof(float *) * (*size + 1));
    if (tab == NULL)
        return (NULL);
    for (int i = 0; *size > i; ++i) {
        tab[i] = malloc(sizeof(float) * cols);
        if (tab[i] == NULL)
            return (NULL);
    }
    tab[2][0] = 2;
    tab[*size] = NULL;

    char **str = get_each_elem(src, elem);
    for (int i = 0; str[i] ; ++i) {
        tab[l][c] = atof(str[i]);
        ++c;
        if (c == cols) {
            c = 0;
            ++l;
        }
    }
    for (int i = 0; elem > i; ++i)
        free(str[i]);
    free(str);
    return (tab);
}

void *decrypt(char **av)
{
    float **tab;
    float **tab1;
    float **tab2;
    char *str;
    int size1;
    int size2 = 0;
    int cols = 0;

    tab2 = key_into_numbers(av[2], &size2, &cols);
    tab2 = inv_matrix(tab2, size2);
    if (tab2 == NULL)
        return (NULL);
    size1 = size2;
    disp_key(tab2, size2, 16543);
    tab1 = into_the_matrix(av[1], &size1, cols);
    if (tab1 == NULL)
        return (NULL);
    tab = matrix_mul(tab1, tab2, size1, size2);
    str = matrix_to_str(tab, size1, size2);
    printf("Decrypted message:\n");
    printf("%s\n", str);
    free(str);
    for (int i = 0 ; tab[i] ; i++)
        free(tab[i]);
    free(tab);
}

int main(int ac, char **av)
{
    if (ac == 2) {
        if ('-' == av[1][0] && 'h' == av[1][1]) {
            disp_hflag(av);
            return (84);
        }
    }
    if (84 == deal_error(ac, av))
        return (84);
    if ('0' == av[3][0])
        encrypt(av);
    if ('1' == av[3][0])
        decrypt(av);
    return (0);
}
