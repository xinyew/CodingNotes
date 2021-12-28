

int main() {
    union u_tag {
        int ival;
        float fval;
        char *sval;
    } *u;
    u -> ival = 10;
}