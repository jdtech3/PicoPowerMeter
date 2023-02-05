# PicoPowerMeter

## ?¿

Voltage/current/power/energy meter using the Raspberry Pi Pico, aiming to be ± 0.1% @ 0-24V and 0-1A

## Pics

PCB:

<img src="https://user-images.githubusercontent.com/8998191/211696509-9db38457-eb5d-4b87-a962-af368a12de93.png" width=50% height=50%>

Prototype:

*[TBD]*

Completed:

*[TBD]*

## Calibration

Even when oversampling by averaging over 10,000 samples, we noticed that there were significant errors (up to ± 10%) with the voltage measurements:

<img src="https://user-images.githubusercontent.com/8998191/211696514-ade1334f-68c2-412e-8fbf-e7e54c42ac00.png" width=50% height=50%>

This seems to be due to the poor DNL and INL (differential and integral non-linearity respectively) of the Raspberry Pi Pico ADC, as our measured "jumps" correspond almost exactly to the Pico's DNL spikes as measured by [Mark Omo's excellent characterization of the Pico's ADC](https://pico-adc.markomo.me/INL-DNL/).

Methodology and raw calibration data for our Pico is available [here](https://docs.google.com/spreadsheets/d/1IjrpbZwi6U-m_iKFwJcBlV5J3Jlb0WuwdMCl5kuGMQ8/edit?usp=sharing).

## Contributors

* [Joe Dai](https://github.com/jdtech3)
* [Gary Zhou](https://github.com/GaryZhous/)

---

*This project is licensed under NONE, TBD. For more information, please see TBD.*
