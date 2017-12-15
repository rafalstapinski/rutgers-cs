char *column;
char *host;
char *port;

int invalid_file(const char *path);
void get_result(const char *output_dir, const char *column);
void traverse(const char *path);
int main(int argc, char *argv[]);
