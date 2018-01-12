struct map initmap(FILE *input, int w, int h);
void printmap(struct map input);
void cell_on(struct map *m, int i);
void cell_off(struct map *m, int i);
void cell_toggle(struct map *m, int i);
int sum_neighbors(struct map m, int n);
int evaluate(struct map m, int i);
int coord_to_array(int x, int y, int w, int h);
int find_neighbor(int n, int i, int w, int h);
