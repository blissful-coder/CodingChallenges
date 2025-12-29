// Token types
const TokenType = {
    LBRACE: 'LBRACE',
    RBRACE: 'RBRACE',
    LBRACKET: 'LBRACKET',
    RBRACKET: 'RBRACKET',
    STRING: 'STRING',
    COLON: 'COLON',
    COMMA: 'COMMA',
    NUMBER: 'NUMBER',
    TRUE: 'TRUE',
    FALSE: 'FALSE',
    NULL: 'NULL',
    EOF: 'EOF',
    INVALID: 'INVALID'
};

// Node types for parse tree
const NodeType = {
    OBJECT: 'OBJECT',
    ARRAY: 'ARRAY',
    STRING: 'STRING',
    NUMBER: 'NUMBER',
    BOOLEAN: 'BOOLEAN',
    NULL: 'NULL'
};

class Token {
    constructor(type, value = '', pos = 0) {
        this.type = type;
        this.value = value;
        this.pos = pos;
    }
}

class ParseTreeNode {
    constructor(type, value = '', key = '') {
        this.type = type;
        this.value = value;
        this.key = key;
        this.children = new Map();
        this.arrayItems = [];
    }

    addChild(key, child) {
        this.children.set(key, child);
    }

    addArrayItem(item) {
        this.arrayItems.push(item);
    }

    getDisplayValue() {
        switch (this.type) {
            case NodeType.OBJECT:
                return `Object (${this.children.size} members)`;
            case NodeType.ARRAY:
                return `Array (${this.arrayItems.length} items)`;
            case NodeType.STRING:
                return `"${this.value}"`;
            case NodeType.NUMBER:
                return this.value;
            case NodeType.BOOLEAN:
                return this.value;
            case NodeType.NULL:
                return 'null';
            default:
                return 'Unknown';
        }
    }

    getNodeTypeString() {
        switch (this.type) {
            case NodeType.OBJECT: return 'Object';
            case NodeType.ARRAY: return 'Array';
            case NodeType.STRING: return 'String';
            case NodeType.NUMBER: return 'Number';
            case NodeType.BOOLEAN: return 'Boolean';
            case NodeType.NULL: return 'Null';
            default: return 'Unknown';
        }
    }
}

class Lexer {
    constructor(input) {
        this.input = input;
        this.pos = 0;
    }

    skipWhitespace() {
        while (this.pos < this.input.length && /\s/.test(this.input[this.pos])) {
            this.pos++;
        }
    }

    next() {
        this.skipWhitespace();
        if (this.pos >= this.input.length) {
            return new Token(TokenType.EOF, '', this.pos);
        }

        const char = this.input[this.pos];
        const startPos = this.pos;

        switch (char) {
            case '{':
                this.pos++;
                return new Token(TokenType.LBRACE, '{', startPos);
            case '}':
                this.pos++;
                return new Token(TokenType.RBRACE, '}', startPos);
            case '[':
                this.pos++;
                return new Token(TokenType.LBRACKET, '[', startPos);
            case ']':
                this.pos++;
                return new Token(TokenType.RBRACKET, ']', startPos);
            case ':':
                this.pos++;
                return new Token(TokenType.COLON, ':', startPos);
            case ',':
                this.pos++;
                return new Token(TokenType.COMMA, ',', startPos);
            case '"':
                return this.parseString();
            default:
                if (/[a-zA-Z]/.test(char)) {
                    return this.parseKeyword();
                } else if (/[\d-]/.test(char)) {
                    return this.parseNumber();
                } else {
                    this.pos++;
                    return new Token(TokenType.INVALID, char, startPos);
                }
        }
    }

    parseString() {
        const startPos = this.pos;
        this.pos++; // skip opening quote
        let value = '';

        while (this.pos < this.input.length && this.input[this.pos] !== '"') {
            if (this.input[this.pos] === '\\') {
                this.pos++; // skip escape character
                if (this.pos >= this.input.length) {
                    return new Token(TokenType.INVALID, 'Unterminated string', startPos);
                }
                // Handle common escape sequences
                switch (this.input[this.pos]) {
                    case 'n': value += '\n'; break;
                    case 't': value += '\t'; break;
                    case 'r': value += '\r'; break;
                    case '\\': value += '\\'; break;
                    case '"': value += '"'; break;
                    case '/': value += '/'; break;
                    default: value += this.input[this.pos]; break;
                }
                this.pos++;
            } else {
                value += this.input[this.pos];
                this.pos++;
            }
        }

        if (this.pos >= this.input.length) {
            return new Token(TokenType.INVALID, 'Unterminated string', startPos);
        }

        this.pos++; // skip closing quote
        return new Token(TokenType.STRING, value, startPos);
    }

    parseKeyword() {
        const startPos = this.pos;
        let value = '';

        while (this.pos < this.input.length && /[a-zA-Z]/.test(this.input[this.pos])) {
            value += this.input[this.pos];
            this.pos++;
        }

        let type = TokenType.INVALID;
        switch (value) {
            case 'true':
                type = TokenType.TRUE;
                break;
            case 'false':
                type = TokenType.FALSE;
                break;
            case 'null':
                type = TokenType.NULL;
                break;
        }

        return new Token(type, value, startPos);
    }

    parseNumber() {
        const startPos = this.pos;
        let value = '';

        // Handle negative numbers
        if (this.input[this.pos] === '-') {
            value += '-';
            this.pos++;
        }

        // Parse integer part
        while (this.pos < this.input.length && /\d/.test(this.input[this.pos])) {
            value += this.input[this.pos];
            this.pos++;
        }

        // Parse decimal part
        if (this.pos < this.input.length && this.input[this.pos] === '.') {
            value += '.';
            this.pos++;
            while (this.pos < this.input.length && /\d/.test(this.input[this.pos])) {
                value += this.input[this.pos];
                this.pos++;
            }
        }

        // Parse exponent part
        if (this.pos < this.input.length && /[eE]/.test(this.input[this.pos])) {
            value += this.input[this.pos];
            this.pos++;
            if (this.pos < this.input.length && /[+-]/.test(this.input[this.pos])) {
                value += this.input[this.pos];
                this.pos++;
            }
            while (this.pos < this.input.length && /\d/.test(this.input[this.pos])) {
                value += this.input[this.pos];
                this.pos++;
            }
        }

        return new Token(TokenType.NUMBER, value, startPos);
    }
}

class JsonParser {
    constructor(input) {
        this.lexer = new Lexer(input);
        this.currentToken = new Token(TokenType.INVALID);
        this.advance();
    }

    advance() {
        this.currentToken = this.lexer.next();
    }

    parse() {
        if (this.currentToken.type !== TokenType.LBRACE) {
            throw new Error('JSON must start with {');
        }
        return this.parseObject();
    }

    parseObject() {
        if (this.currentToken.type !== TokenType.LBRACE) {
            throw new Error('Expected {');
        }
        this.advance();

        const objNode = new ParseTreeNode(NodeType.OBJECT);

        if (this.currentToken.type === TokenType.RBRACE) {
            // empty object
            this.advance();
            return objNode;
        }

        // Parse first key-value pair
        if (this.currentToken.type !== TokenType.STRING) {
            throw new Error('Expected string key');
        }

        const key = this.currentToken.value;
        this.advance();

        const value = this.parseKeyValue(key);
        if (!value) {
            throw new Error('Failed to parse key-value pair');
        }

        objNode.addChild(key, value);

        // Parse additional key-value pairs
        while (this.currentToken.type === TokenType.COMMA) {
            this.advance(); // consume comma

            // Check for trailing comma (invalid)
            if (this.currentToken.type === TokenType.RBRACE) {
                throw new Error('Trailing comma not allowed');
            }

            if (this.currentToken.type !== TokenType.STRING) {
                throw new Error('Expected string key');
            }

            const nextKey = this.currentToken.value;
            this.advance();

            const nextValue = this.parseKeyValue(nextKey);
            if (!nextValue) {
                throw new Error('Failed to parse key-value pair');
            }

            objNode.addChild(nextKey, nextValue);
        }

        if (this.currentToken.type !== TokenType.RBRACE) {
            throw new Error('Expected }');
        }
        this.advance();

        return objNode;
    }

    parseKeyValue(key) {
        // Parse colon
        if (this.currentToken.type !== TokenType.COLON) {
            throw new Error('Expected :');
        }
        this.advance();

        // Parse value
        const value = this.parseValue();
        if (value) {
            value.key = key;
        }
        return value;
    }

    parseValue() {
        switch (this.currentToken.type) {
            case TokenType.TRUE:
                this.advance();
                return new ParseTreeNode(NodeType.BOOLEAN, 'true');

            case TokenType.FALSE:
                this.advance();
                return new ParseTreeNode(NodeType.BOOLEAN, 'false');

            case TokenType.NULL:
                this.advance();
                return new ParseTreeNode(NodeType.NULL, 'null');

            case TokenType.NUMBER:
                const numValue = this.currentToken.value;
                this.advance();
                return new ParseTreeNode(NodeType.NUMBER, numValue);

            case TokenType.STRING:
                const strValue = this.currentToken.value;
                this.advance();
                return new ParseTreeNode(NodeType.STRING, strValue);

            case TokenType.LBRACE:
                return this.parseObject();

            case TokenType.LBRACKET:
                return this.parseArray();

            default:
                throw new Error(`Unexpected token: ${this.currentToken.type}`);
        }
    }

    parseArray() {
        if (this.currentToken.type !== TokenType.LBRACKET) {
            throw new Error('Expected [');
        }
        this.advance();

        const arrayNode = new ParseTreeNode(NodeType.ARRAY);

        if (this.currentToken.type === TokenType.RBRACKET) {
            // empty array
            this.advance();
            return arrayNode;
        }

        // Parse first value
        const value = this.parseValue();
        if (!value) {
            throw new Error('Failed to parse array value');
        }

        arrayNode.addArrayItem(value);

        // Parse additional values
        while (this.currentToken.type === TokenType.COMMA) {
            this.advance(); // consume comma

            // Check for trailing comma (invalid)
            if (this.currentToken.type === TokenType.RBRACKET) {
                throw new Error('Trailing comma not allowed');
            }

            const nextValue = this.parseValue();
            if (!nextValue) {
                throw new Error('Failed to parse array value');
            }

            arrayNode.addArrayItem(nextValue);
        }

        if (this.currentToken.type !== TokenType.RBRACKET) {
            throw new Error('Expected ]');
        }
        this.advance();

        return arrayNode;
    }
}
