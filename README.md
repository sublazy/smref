# A reference implementation of a statemachine

This implementation has two main features: very low latencies, and no dynamic
memory allocation. As such, it is targeted mainly at bare-metal software
running on microcontrollers.

## How it works [TODO]

- FSM engine
- responsible for checking events, performing transitions and executing
  functions appropriate for the current state.
- for each state machine:
    - define states and events in a header
    - define state transitions in a state transition table
    - define logic (`on_entry`, `on_exit`, `on_tick` function) and bind them to
      the transition table.

## Automating FSM creation process

After implementing a few state machines, it becomes tedious to manually write
FSM files, always following the same pattern. So a generator is provided. It
allows to specify states, events, and transition in an xml file:

```
<?xml version = "1.0" ?>
<fsm
    name        = "Traffic Lights Controller"
    abbr        = "tlc"
    script      = "fsm.gsl"
    first_state = "off"
>

<events>
    <event name = "reset" />
    <event name = "next" />
</events>

<states>
    <state name = "off">
        <transition event = "next"  new_state = "red" />
    </state>

    <state name = "red">
        <transition event = "next"  new_state = "yellow" />
        <transition event = "reset" new_state = "off" />
    </state>

    <state name = "yellow">
        <transition event = "next"  new_state = "green" />
        <transition event = "reset" new_state = "off" />
    </state>

    <state name = "green">
        <transition event = "next"  new_state = "red" />
        <transition event = "reset" new_state = "off" />
    </state>
</states>

</fsm>
```

Having the state machine specified as an GSL model, allows to automatically generate code which implements states, events, and the transition table, as well as the template of functions associated with each state.
```
> gsl -script:fsmgen.gsl fsm_tlc.xml
```

We can also generate a plantUML diagram:
```
> gsl -script:fsm_diagram.gsl fsm_tlc.xml
```

![TLC state transition diagram](fsm_diagram.png)

_Note: Mention similarity of the XML model to SCXML._


