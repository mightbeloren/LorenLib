#include "vector.h"
#include <ctype.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>
#ifndef XML
#define XML

typedef struct {
    size_t index;
    size_t length;
    const char *content;
    arena *allocator;
} Tokenizer;

typedef enum {
    TOKEN_OPEN_TAG,
    TOKEN_CLOSE_TAG,
    TOKEN_TEXT,
    TOKEN_SELF_CLOSING,
    TOKEN_EOF,
} TokenType;

typedef struct {
    const char *key;
    const char *value;
} Attribute;

typedef struct {
    TokenType type;
    vector *attributes; // vector of Attribute
    const char *tag_name;
    const char *text;
} Token;

typedef struct XmlNode {
    const char *name;
    const char *text;
    vector *attributes; // vector of Attribute
    size_t attributes_count;
    vector *children; // vector of XmlNode
    size_t childrens_count;
} XmlNode;

typedef struct {
    XmlNode *root;
    arena *allocator;
} Xml;

XmlNode *parse_content(Xml *xml, const char *xml_content);
Token tokenize(Tokenizer *tokenizer);
const char *parse_name(Tokenizer *t);
const char *parse_text(Tokenizer *t);
vector *parse_attributes(Tokenizer *t);

Xml *xml_parse(const char *xml_content, arena *allocator) {
    if (xml_content == NULL) {
        printf("xml content is null\n");
        return NULL;
    }
    if (allocator == NULL) {
        printf("allocator is not initialized\n");
        return NULL;
    }
    Xml *xml = (Xml *)arena_alloc(allocator, sizeof(Xml));
    if (xml == NULL) {
        printf("unable to allocate xml parser instance\n");
        return NULL;
    }
    xml->allocator = allocator;
    xml->root = parse_content(xml, xml_content);
    return xml;
}

XmlNode *parse_content(Xml *xml, const char *xml_content) {
    Tokenizer tokenizer = {.index = 0,
        .length = strlen(xml_content),
        .content = xml_content,
        .allocator = xml->allocator};
    while (true) {
        Token token = tokenize(&tokenizer);
        switch (token.type) {

            case TOKEN_OPEN_TAG:
            printf("TOKEN : OPENTAG, Name : %s\n", token.tag_name);
            break;

            case TOKEN_TEXT:
            printf("TOKEN : TEXT, Name : %s\n", token.text);
            break;

            case TOKEN_CLOSE_TAG:
            printf("TOKEN : CLOSETAG, Name : %s\n", token.tag_name);
            break;

            case TOKEN_SELF_CLOSING:
            printf("TOKEN : SELFCLOSINGTAG, Name : %s\n", token.tag_name);
            break;

            case TOKEN_EOF:
            break;
        }
        if (token.type == TOKEN_EOF) {
            printf("Tokenization done");
            break;
        }
    }
    return NULL;
}

Token tokenize(Tokenizer *t) {
    Token token;
    while (t->index < t->length && isspace(t->content[t->index])) {
        t->index++;
    }
    if (t->index >= t->length) {
        token.type = TOKEN_EOF;
        return token;
    }

    if (t->content[t->index] == '<') {
        t->index++; // consume <

        if (t->content[t->index] == '/') { // CLOSE TAG

            t->index++; // consume /
            token.type = TOKEN_CLOSE_TAG;
            token.tag_name = parse_name(t); // read until >
            t->index++;                     // consume >
        } else {                          // OPENTAG or SELF CLOSING TAG
            token.type = TOKEN_OPEN_TAG;
            token.tag_name = parse_name(t); // read until space or > or /
            token.attributes = parse_attributes(t);

            if (t->content[t->index] == '/') {
                token.type = TOKEN_SELF_CLOSING;
                t->index++; // consume /
            }
            t->index++; // consume >
        }
    } else {
        token.type = TOKEN_TEXT;
        token.text = parse_text(t); // read until <
    }
    return token;
}

const char *parse_name(Tokenizer *t) {
    size_t start_index;
    size_t end_index;
    while (t->index < t->length && isspace(t->content[t->index])) {
        t->index++;
    }
    start_index = t->index;
    while (t->index < t->length && t->content[t->index] != ' ' &&
        t->content[t->index] != '/' && t->content[t->index] != '>') {
            t->index++;
        }
    end_index = t->index;
    size_t length = end_index - start_index;
    char *tag_name = (char *)arena_alloc(t->allocator, length + 1);
    strncpy(tag_name, t->content + start_index, length);
    tag_name[length] = '\0';
    return tag_name;
}
const char *parse_text(Tokenizer *t) {
    size_t start_index;
    size_t end_index;
    start_index = t->index;
    while (t->index < t->length && t->content[t->index] != '<') {
        t->index++;
    }
    end_index = t->index;
    size_t length = end_index - start_index;
    char *tag_text = (char *)arena_alloc(t->allocator, length + 1);
    strncpy(tag_text, t->content + start_index, length);
    tag_text[length] = '\0';
    return tag_text;
}

vector *parse_attributes(Tokenizer *t) {
    vector *attributes = vec_new(sizeof(Attribute), 10, t->allocator);
    // for now skip attributes parsing and consume till / or >
    while (t->index < t->length && t->content[t->index] != '/' &&
        t->content[t->index] != '>') {
            t->index++;
        }
    return attributes;
}

#endif
