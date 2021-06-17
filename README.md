# regexsentenses
This program takes in a regex expression and outputs all possible sentences that match the expression.

# Quick Set Up
```
make install
regexsentenses "[0-9A-F]{1:3}\n"
```

# Examples
To make random css
```
regexsentenses "\.[a-z]{3}\{\n\t[(background\-color)(color)]: #[0-9A-F]{6};\n\}\n"
```

Cracking John Smiths super secret password
```
regexsentenses "[jJ]ohn[sS]mith[0-9]{0:2}[!@#$%]{0:1}\n" | \
    xargs -P0 -I % sh -c \
        'if [ "$(echo '%' | openssl dgst -hex -sha256)" = "55fc540c73118d224e8fa3523761ddde0a6d626d18896bea6afb2eb748c00b78" ]; then echo '%'; fi'
```

To calculate the total of all possible combination of 3 digits including an empty character
```
echo $(regexsentenses '\[\([0-9]{1:3}\\\+\)\]')"0\n" | regexsentenses | bc
```

To print all numbers starting with 0 or 1
```
regexsentenses "[1-9][0-9]*\n"
regexsentenses "[0-9]+\n"
```

Test all possibilities to a regex expression
```
regexsentenses "if *\(.*\)\{\n"
regexsentenses -c 2 "for[ \t\n]+\([ \t\n]*int[ \t\n]+i[ \t\n]*=[ \t\n]*0[ \t\n]*;[ \t\n]*i[ \t\n]*<[a-zA-Z0-9 \t\n]*;[ \t\n]*i[ \t\n]*\+\+[ \t\n]*;[ \t\n]*\)[ \t\n]*\{\n"
```


# Install
Install to /usr/local/bin/
```
make install
```

# Uninstall
```
make uninstall
```

# Build
```
make build
```

# Clean
```
make clean
```

# Run Through Make
```
echo "[0-9A-F]+\n" | make run clean
```

