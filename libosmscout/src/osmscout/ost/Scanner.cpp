
/*
  This source is part of the libosmscout library
  Copyright (C) 2011  Tim Teulings

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307  USA
*/

#include <cstring>

#include <osmscout/ost/Scanner.h>

namespace osmscout {
namespace ost {


// string handling, wide character

char* coco_string_create(const char* value) {
  char* data;
  int len = 0;
  if (value) { len = strlen(value); }
  data = new char[len + 1];
  strncpy(data, value, len);
  data[len] = 0;
  return data;
}

char* coco_string_create(const char *value , int startIndex, int length) {
  int len = 0;
  char* data;

  if (value) { len = length; }
  data = new char[len + 1];
  strncpy(data, &(value[startIndex]), len);
  data[len] = 0;

  return data;
}

void coco_string_delete(char* &data) {
  delete [] data;
  data = NULL;
}

Token::Token()
{
  kind = 0;
  pos  = 0;
  col  = 0;
  line = 0;
  val  = NULL;
  next = NULL;
}

Token::~Token()
{
  coco_string_delete(val);
}


Buffer::Buffer(const unsigned char* buf, int len)
{
  this->buf = new unsigned char[len];
  memcpy(this->buf, buf, len*sizeof(unsigned char));
  bufLen = len;
  bufPos = 0;
}

Buffer::~Buffer()
{
  delete [] buf;
}

int Buffer::Peek()
{
  int curPos = GetPos();
  int ch = Read();
  SetPos(curPos);
  return ch;
}

/*
char* Buffer::GetString(int beg, int end)
{
  int len = 0;
  char *buf = new char[end - beg];
  int oldPos = GetPos();
  SetPos(beg);
  while (GetPos() < end) buf[len++] = (char) Read();
  SetPos(oldPos);
  char *res = coco_string_create(buf, 0, len);
  coco_string_delete(buf);
  return res;
}*/

int Buffer::GetPos()
{
  return bufPos;
}

void Buffer::SetPos(int value) {
  if ((value < 0) || (value > bufLen)) {
    printf("--- buffer out of bounds access, position: %d\n", value);
    exit(1);
  }

  bufPos = value;
}

int Buffer::Read()
{
  if (bufPos < bufLen) {
    return buf[bufPos++];
  } else {
    return EoF;
  }
}

Scanner::Scanner(const unsigned char* buf, int len) {
  buffer = new Buffer(buf, len);
  Init();
}

Scanner::~Scanner() {
  char* cur = (char*) firstHeap;

  while(cur != NULL) {
    cur = *(char**) (cur + HEAP_BLOCK_SIZE);
    free(firstHeap);
    firstHeap = cur;
  }
  delete [] tval;
  delete buffer;
}

void Scanner::Init() {
  EOL    = '\n';
  eofSym = 0;
	maxT = 19;
	noSym = 19;
	int i;
	for (i = 65; i <= 90; ++i) start.set(i, 1);
	for (i = 97; i <= 122; ++i) start.set(i, 1);
	for (i = 48; i <= 57; ++i) start.set(i, 2);
	start.set(34, 3);
	start.set(61, 7);
		start.set(Buffer::EoF, -1);
	keywords.set("OST", 4);
	keywords.set("END", 5);
	keywords.set("TYPES", 6);
	keywords.set("TYPE", 7);
	keywords.set("WHERE", 8);
	keywords.set("CAN", 10);
	keywords.set("BE", 11);
	keywords.set("NODE", 12);
	keywords.set("WAY", 13);
	keywords.set("AREA", 14);
	keywords.set("RELATION", 15);
	keywords.set("OPTIONS", 16);
	keywords.set("ROUTE", 17);
	keywords.set("INDEX", 18);


  tvalLength = 128;
  tval = new char[tvalLength]; // text of current token

  // HEAP_BLOCK_SIZE byte heap + pointer to next heap block
  heap = malloc(HEAP_BLOCK_SIZE + sizeof(void*));
  firstHeap = heap;
  heapEnd = (void**) (((char*) heap) + HEAP_BLOCK_SIZE);
  *heapEnd = 0;
  heapTop = heap;
  if (sizeof(Token) > HEAP_BLOCK_SIZE) {
    printf("--- Too small HEAP_BLOCK_SIZE\n");
    exit(1);
  }

  pos = -1; line = 1; col = 0;
  oldEols = 0;
  NextCh();
  if (ch == 0xEF) { // check optional byte order mark for UTF-8
    NextCh(); int ch1 = ch;
    NextCh(); int ch2 = ch;
    if (ch1 != 0xBB || ch2 != 0xBF) {
      printf("Illegal byte order mark at start of file");
      exit(1);
    }
    NextCh();
  }


  pt = tokens = CreateToken(); // first token is a dummy
}

void Scanner::NextCh() {
  if (oldEols > 0) {
    ch = EOL;
    oldEols--;
  }
  else {
    pos = buffer->GetPos();
    ch = buffer->Read(); col++;
    // replace isolated '\r' by '\n' in order to make
    // eol handling uniform across Windows, Unix and Mac
    if (ch == '\r' && buffer->Peek() != '\n') {
      ch = EOL;
    }
    if (ch == EOL) {
      line++;
      col = 0;
    }
  }

}

void Scanner::AddCh() {
  if (tlen >= tvalLength) {
    tvalLength *= 2;
    char *newBuf = new char[tvalLength];
    memcpy(newBuf, tval, tlen*sizeof(char));
    delete [] tval;
    tval = newBuf;
  }
  if (ch != Buffer::EoF) {
		tval[tlen++] = ch;
    NextCh();
  }
}


bool Scanner::Comment0() {
	int level = 1, pos0 = pos, line0 = line, col0 = col;
	NextCh();
	if (ch == '/') {
		NextCh();
		for(;;) {
			if (ch == 10) {
				level--;
				if (level == 0) { oldEols = line - line0; NextCh(); return true; }
				NextCh();
			} else if (ch == buffer->EoF) return false;
			else NextCh();
		}
	} else {
		buffer->SetPos(pos0); NextCh(); line = line0; col = col0;
	}
	return false;
}

bool Scanner::Comment1() {
	int level = 1, pos0 = pos, line0 = line, col0 = col;
	NextCh();
	if (ch == '*') {
		NextCh();
		for(;;) {
			if (ch == '*') {
				NextCh();
				if (ch == '/') {
					level--;
					if (level == 0) { oldEols = line - line0; NextCh(); return true; }
					NextCh();
				}
			} else if (ch == '/') {
				NextCh();
				if (ch == '*') {
					level++; NextCh();
				}
			} else if (ch == buffer->EoF) return false;
			else NextCh();
		}
	} else {
		buffer->SetPos(pos0); NextCh(); line = line0; col = col0;
	}
	return false;
}


void Scanner::CreateHeapBlock() {
  void* newHeap;
  char* cur = (char*) firstHeap;

  while(((char*) tokens < cur) || ((char*) tokens > (cur + HEAP_BLOCK_SIZE))) {
    cur = *((char**) (cur + HEAP_BLOCK_SIZE));
    free(firstHeap);
    firstHeap = cur;
  }

  // HEAP_BLOCK_SIZE byte heap + pointer to next heap block
  newHeap = malloc(HEAP_BLOCK_SIZE + sizeof(void*));
  *heapEnd = newHeap;
  heapEnd = (void**) (((char*) newHeap) + HEAP_BLOCK_SIZE);
  *heapEnd = 0;
  heap = newHeap;
  heapTop = heap;
}

Token* Scanner::CreateToken() {
  Token *t;
  if (((char*) heapTop + (int) sizeof(Token)) >= (char*) heapEnd) {
    CreateHeapBlock();
  }
  t = (Token*) heapTop;
  heapTop = (void*) ((char*) heapTop + sizeof(Token));
  t->val = NULL;
  t->next = NULL;
  return t;
}

void Scanner::AppendVal(Token *t) {
  int reqMem = (tlen + 1) * sizeof(char);
  if (((char*) heapTop + reqMem) >= (char*) heapEnd) {
    if (reqMem > HEAP_BLOCK_SIZE) {
      printf("--- Too long token value\n");
      exit(1);
    }
    CreateHeapBlock();
  }
  t->val = (char*) heapTop;
  heapTop = (void*) ((char*) heapTop + reqMem);

  strncpy(t->val, tval, tlen);
  t->val[tlen] = '\0';
}

Token* Scanner::NextToken() {
  while (ch == ' ' ||
			(ch >= 9 && ch <= 10) || ch == 13
  ) NextCh();
	if ((ch == '/' && Comment0()) || (ch == '/' && Comment1())) return NextToken();
  t = CreateToken();
  t->pos = pos; t->col = col; t->line = line;
  int state = start.state(ch);
  tlen = 0; AddCh();

  switch (state) {
    case -1: { t->kind = eofSym; break; } // NextCh already done
    case 0: { t->kind = noSym; break; }   // NextCh already done
		case 1:
			case_1:
			if ((ch >= 'A' && ch <= 'Z') || ch == '_' || (ch >= 'a' && ch <= 'z')) {AddCh(); goto case_1;}
			else {t->kind = 1; char *literal = coco_string_create(tval, 0, tlen); t->kind = keywords.get(literal, t->kind); coco_string_delete(literal); break;}
		case 2:
			case_2:
			if ((ch >= '0' && ch <= '9')) {AddCh(); goto case_2;}
			else {t->kind = 2; break;}
		case 3:
			case_3:
			if (ch <= '!' || (ch >= '#' && ch <= '[') || (ch >= ']' && ch <= 65535)) {AddCh(); goto case_3;}
			else if (ch == '"') {AddCh(); goto case_4;}
			else if (ch == 92) {AddCh(); goto case_5;}
			else {t->kind = noSym; break;}
		case 4:
			case_4:
			{t->kind = 3; break;}
		case 5:
			case_5:
			if (ch <= '!' || (ch >= '#' && ch <= '[') || (ch >= ']' && ch <= 65535)) {AddCh(); goto case_3;}
			else if (ch == 92) {AddCh(); goto case_5;}
			else if (ch == '"') {AddCh(); goto case_6;}
			else {t->kind = noSym; break;}
		case 6:
			case_6:
			if (ch <= '!' || (ch >= '#' && ch <= '[') || (ch >= ']' && ch <= 65535)) {AddCh(); goto case_3;}
			else if (ch == '"') {AddCh(); goto case_4;}
			else if (ch == 92) {AddCh(); goto case_5;}
			else {t->kind = 3; break;}
		case 7:
			if (ch == '=') {AddCh(); goto case_8;}
			else {t->kind = noSym; break;}
		case 8:
			case_8:
			{t->kind = 9; break;}

  }
  AppendVal(t);

  return t;
}

// get the next token (possibly a token already seen during peeking)
Token* Scanner::Scan() {
  if (tokens->next == NULL) {
    return pt = tokens = NextToken();
  } else {
    pt = tokens = tokens->next;
    return tokens;
  }
}

// peek for the next token, ignore pragmas
Token* Scanner::Peek() {
  do {
    if (pt->next == NULL) {
      pt->next = NextToken();
    }
    pt = pt->next;
  } while (pt->kind > maxT); // skip pragmas

  return pt;
}

// make sure that peeking starts at the current scan position
void Scanner::ResetPeek() {
  pt = tokens;
}

} // namespace
} // namespace

