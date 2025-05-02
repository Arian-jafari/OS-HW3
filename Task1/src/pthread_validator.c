#include "pthread_validator.h"

typedef struct {
    int table[9][9];
    bool result;
} ThreadData;

bool check_rows(int table[9][9]) {
    bool seen[10] = {false};
    bool satisified = true;

    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            int num = table[i][j];
            if (num < 1 || num > 9 || seen[num]) {
                satisified = false;
                break;
            } else {
                seen[table[i][j]] = true;
            }
        }
        if (!satisified) 
            return false;
        memset(seen, false, sizeof(seen));
    }
    return true;
}

bool check_columns(int table[9][9]) {
    bool seen[10] = {false};
    bool satisified = true;

    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            int num = table[j][i];
            if (num < 1 || num > 9 || seen[num]) {
                satisified = false;
                break;
            } else {
                seen[table[j][i]] = true;
            }
        }
        if (!satisified) 
            return false;
        memset(seen, false, sizeof(seen));
    }
    return true;
}
bool check_boxes(int table[9][9]) {
    bool seen[10] = {false};
    bool satisified = true;

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            for (int k = 0; k < 3; k++) {
                for (int l = 0; l < 3; l++) {
                    int num = table[i * 3 + k][j * 3 + l];
                    if (num < 1 || num > 9 || seen[num]) {
                        satisified = false;
                        break;
                    } else {
                        seen[table[i * 3 + k][j * 3 + l]] = true;
                    }
                }
            }
            if (!satisified) 
                return false;
            memset(seen, false, sizeof(seen));
        }
    }
    return true;
}

void* check_rows_wrapper(void* arg) {
    ThreadData* data = (ThreadData*)arg;
    data->result = check_rows(data->table);
    return NULL;
}

void* check_columns_wrapper(void* arg) {
    ThreadData* data = (ThreadData*)arg;
    data->result = check_columns(data->table);
    return NULL;
}

void* check_boxes_wrapper(void* arg) {
    ThreadData* data = (ThreadData*)arg;
    data->result = check_boxes(data->table);
    return NULL;
}

bool sudoku_validator(int table[9][9]) {
    pthread_t row_thread, column_thread, box_thread;
    ThreadData row_data, column_data, box_data;
    bool final_result = true;

    memcpy(row_data.table, table, sizeof(int) * 9 * 9);
    memcpy(column_data.table, table, sizeof(int) * 9 * 9);
    memcpy(box_data.table, table, sizeof(int) * 9 * 9);

    
    pthread_create(&row_thread, NULL, check_rows_wrapper, &row_data);
    pthread_create(&column_thread, NULL, check_columns_wrapper, &column_data);
    pthread_create(&box_thread, NULL, check_boxes_wrapper, &box_data);

    pthread_join(row_thread, NULL);
    pthread_join(column_thread, NULL);
    pthread_join(box_thread, NULL);

    final_result = row_data.result && column_data.result && box_data.result;

    return final_result;
}
