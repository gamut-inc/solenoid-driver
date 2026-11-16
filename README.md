# Solenoid

A small Arduino library for playing magnets from MIDI: beaters that strike,
dampers that mute, and magnets that are held with variable force.

It is shared by the firmware of several of gamut inc's music machines. Each
instrument is one sketch that lists its magnets; the rest is this library. The version here, 0.11, is the one in the sketch folders dated
2020–21.

## The idea

A solenoid that strikes a bar or a drum skin is not switched on and off like a
light. It gets a pulse of a few milliseconds, and the length of that pulse is
the dynamics: too short and the beater does not reach, too long and it stays on
the surface and chokes the sound. The usable range is different for every
magnet, every spring and every instrument.

So each magnet is declared with its own range, and MIDI velocity is mapped onto
it:

```cpp
#include <Solenoid.h>

//                 type    note  pin  ms at vel 1  ms at vel 127  delay ms
Solenoid low  (    BEATER, 60,   2,   3,           20,            0 );
Solenoid high (    BEATER, 61,   3,   3,           20,            0 );
```

```cpp
void loop() {
  if (MIDI.read()) {
    low.control (MIDI.getType(), MIDI.getData1(), MIDI.getData2());
    high.control(MIDI.getType(), MIDI.getData1(), MIDI.getData2());
  }
  low.process();
  high.process();
}
```

`control()` only records when the pulse starts and ends. `process()` compares
those times against `micros()` and sets the pin. Nothing blocks, so sixteen
magnets can overlap freely and incoming MIDI is never held up by a `delay()`.

## Magnet types

| type | behaviour |
|---|---|
| `BEATER` | note-on starts a pulse whose length follows velocity; note-off ends it early |
| `DAMPER` | follows the note of its beater and mutes when the note ends; also playable on its own, two octaves above its trigger note |
| `CONTINOUS` | driven with `analogWrite`; velocity or controller value sets the duty cycle between the two limits, note-off releases |

The `delay` argument postpones the pulse by a fixed time. It exists to line up
magnets with different travel, or a whole instrument with a slower one next to
it.

## Damper modes

Three notes at the bottom of the range switch how dampers behave, for all
magnets at once, so the mode can be changed from the sequencer in the middle of
a piece:

| note | mode | |
|---|---|---|
| 0 | `STANDART` | dampers follow note-off |
| 1 | `FLOATING` | dampers stay off, everything rings |
| 2 | `DAMPED` | damper engages with the strike, and the beater is delayed so that it lands on the damped surface |

Data byte 123 switches a magnet's output low. This is the safety net for the
sequencer being stopped or closed while magnets are held.

## Examples

- `examples/TwoBeaters` — the minimal sketch (comments in German)
- `examples/Trommel` — the sketch of a drum machine as installed: thirteen
  beaters, one continuously driven magnet and an LED on the same mechanism

Both need the [MIDI Library](https://github.com/FortySevenEffects/arduino_midi_library).

## Known limitations

- Pulse times are compared directly against `micros()`, which wraps after about
  71 minutes. A pulse that is active at that moment is cut short, and one
  scheduled across it is lost.
- At most 16 magnets (`MAX_SOLENOIDS`), in a static array without a bounds
  check.
- `STANDART` and `CONTINOUS` are misspelled and are part of the interface;
  existing sketches depend on them.
- `DAMPED` mode overwrites a magnet's `delay` and does not restore it.
- The files were moved from the sketch folders into Arduino library layout for
  this repository. The code is unchanged apart from the include line; the new
  layout has been syntax-checked but not yet built for a board.

## Licence

MIT — see [LICENSE](LICENSE).
