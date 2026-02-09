# Cyfer Architecture

## Internal representation - bytes

Under the hood, **everything** here is just bytes.

* ASCII text → bytes
* HEX string → bytes
* Base64 string → bytes
* Bits string → bytes

Then every cross-conversion is just:

> parse input into bytes → format bytes in another representation

```text
  INPUT TYPES           PROCESSING           OUTPUT TYPES
  ───────────          ────────────          ────────────
  ASCII   ──┐                                ┌── ASCII
  Decimal ──┤          ┌──────────┐          ├── Decimal
  Hex     ──┼──[>>>]──▶│  BYTES   │──▶[>>>]──┼── Hex
  Bits    ──┤          └──────────┘          ├── Bits
  Base64  ──┘                                └── Base64
```

So:

- `ascii2decimal` = `ascii -> bytes -> decimal`
- `hex2base64` = `hex -> bytes -> base64`

---

## Performance

Speed of Chaining Converters:

```bash
cat $ELF | cyfer bytes2base64 | cyfer base642decimal | cyfer decimal2ascii
```

Each primitive converter is **O(n)** over input size (looping chars/bytes). Two or three passes are still **O(n)**.

Try to pass a binary file in `make test`, and you'll see how much time it cost.

---

## Test

Use `OUT_ASCII` as "Golden Standard" to verify consistency across different data paths. Run round-trips for every converters over and over again. Proving f⁻¹(f(x)) = x for every format in toolkit.

- **f(x)**: The **encoder** (`bytes2hex`). It takes raw data x and changes it into a new format.
- **f−1**: The **decoder** (`hex2bytes`). It's the inverse function that does the exact opposite of the first one.
- **=x**: When we run a piece of data through both, we should end up exactly where we started, which is the `OUT_ASCII`.

So the output ASCII stays the same no matter how many times we flip it back and forth.

---

## Error Handling

Mode having `.prpt = NULL` in `cmds[]` array, meaning it **doesn't support interactive mode**, would have error handling to block interactive mode and prompt for user.

Including: representations convert with raw bytes `bytes2...` `...2bytes`.

---

## Auto-Detect Mode

Rule-based detection with confidence scoring:

```
Input String
    ↓
[ is_base64() ]  [ is_hex() ]  [ is_bits() ]  [ is_ascii() ]  [ is_decimal() ]
    ↓                ↓              ↓               ↓                ↓
  true/false      true/false    true/false      true/false       true/false
    ↓
[ Confidence array tracks all matches ]
    ↓
[ Pick first match as primary, collect others as alternatives ]
    ↓
[ Calculate confidence % = 100 / total_matches ]
    ↓
[ Run converter for primary format ]
    ↓
[ Fallback to raw Bytes when nothing else matches]
    ↓
[ List confidence and alternatives ]
```

### is_bits()

Not strictly enforces full bytes only (which is - length must be 8, 16, 24, etc.), but assumes input as tokenizable str.

It tokenizes the input first, then check if every token is: 0 < token_len <= 8 with all digit as 0 or 1.

There are other input would be assuming tokenizable - hex, decimal.

### Order Matters

**ASCII catches everything** because digits, spaces, hex chars (`A-F`), base64 chars are all printable ASCII. That's why we write **detection order** from most-specific to least-specific:

Base64, Decimal, Hex, ASCII, then fallback to raw Bytes.
