
# L_system

## Runnen van de engine <br>
Om de engine te runnen moet je de volgende commando's uitvoeren:

```bash
cmake .
```

```bash
make
```

```bash
mv engine resc/engine
cd resc
./engine
```
## inhoud van de engine  <br>
- Een werkend L-system
- een werkend L-system met stochastiche regels met volgend syntax:
```
Rules = {
    F ->  { "F(+F)F(-F)F" : 0.33, "F(+F)F" : 0.33, "F(-F)F" : 0.34}
}
```

## output van de engine
alle images worden gegeneerd in de map build