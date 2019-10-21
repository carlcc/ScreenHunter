#include <cstdint>
#include <cstdio>
#include <string>

int main(int argc, char** argv)
{
    if (argc != 2) {
        return -1;
    }

    std::string filePath = argv[1];
    FILE* fin = fopen(filePath.c_str(), "rb");

    if (fin == nullptr) {
        perror(filePath.c_str());
        return -1;
    }

    std::string outPath = filePath + ".gen.h";
    FILE* fout = fopen(outPath.c_str(), "w");
    if (fout == nullptr) {
        perror(outPath.c_str());
        return -1;
    }

    fprintf(fout, "#include <cstdint>\n");
    fprintf(fout, "const uint8_t kFontData[] = {");

    uint8_t buf[4096];
    int kBytesPerRow = 64;
    int k = 0;
    while (true) {
        size_t count = fread(buf, 1, sizeof(buf), fin);
        for (int i = 0; i < count; ++i) {
            fprintf(fout, "0x%X,", int(buf[i]));
            if (++k == kBytesPerRow) {
                k = 0;
                fprintf(fout, "\n");
            }
        }
        if (count < sizeof(buf)) {
            break;
        }
    }
    fprintf(fout, "};");

    fclose(fout);
    fclose(fin);
    return 0;
}