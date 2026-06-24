#include "Xml.h"
#include "arena.h"

int main() {
    arena *a = arena_new(1024 * 1024 * 1024);

    const char *xml_content =
        "<Config resolution=128x32>"
        "<Screen duration=5>"
        "<BackgroundColor>#000000</BackgroundColor>"
        "<Grid rows=4 cols=12>"
        "<Cell rowStart=1 colStart=1 rowSpan=2 colSpan=3>"
        "<Image src=logo.bmp/>"
        "</Cell>"
        "<Cell rowStart=1 colStart=4 rowSpan=1 colSpan=9>"
        "<Text font=large align=left valign=top>HELLO</Text>"
        "</Cell>"
        "<Cell rowStart=2 colStart=4 rowSpan=1 colSpan=9>"
        "<Text font=small align=left valign=bottom>world</Text>"
        "</Cell>"
        "</Grid>"
        "</Screen>"
        "</Config>";
    Xml *xml = xml_parse(xml_content, a);
    arena_free(a);
}
