#include <cstdio>

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        printf("usage: %s RULES", argv[0]);
        return 1;
    }

    FILE* rules_f = fopen(argv[1], "r");
    if (!rules_f)
    {
        perror("error opening rules file:");
        return 1;
    }

    std::size_t buflen = 0;
    fseek(rules_f, SEEK_END);
    buflen = ftell(rules_f); 
    fseek(rules_f, SEEK_SET);

    char* buffer = static_cast<char*>(calloc(buflen + 1, sizeof(char)));

    std::string gen_str;

    CGenerator generator;
    generator.gen(std::string_view(buffer, buflen), gen_str);

    printf("%.*s", gen_str.size(), gen_str.data());

    free(buffer); buffer = NULL;
    fclose(rules_f);

    return 0;
}
