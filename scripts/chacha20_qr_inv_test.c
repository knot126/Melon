#define QR(a, b, c, d) \
	a += b; d ^= a; d <<<= 16; \
	c += d; b ^= c; b <<<= 12; \
	a += b; d ^= a; d <<<= 8; \
	c += d; b ^= c; b <<<= 7;

a += b; d ^= a; d <<<= 16;
c += d; b ^= c; b <<<= 12;
a += b; d ^= a; d <<<= 8;
c += d; b ^= c; b <<<= 7;

b >>>= 7; b ^= c; c -= d;
d >>>= 8; d ^= a; a -= b;
b >>>= 12; b ^= c; c -= d;
d >>>= 16; d ^= a; a -= b;

#define QR_INV(a, b, c, d) \
	b >>>= 7; b ^= c; c -= d; \
	d >>>= 8; d ^= a; a -= b; \
	b >>>= 12; b ^= c; c -= d; \
	d >>>= 16; d ^= a; a -= b;
