# Updates
In this version, a critical flaw in the power distribution design has been corrected. Previously, the STM32F469IIT6 microcontroller was powered by an ASM1117-3.3 voltage regulator, stepping down from a 12V input. However, the resulting power dissipation caused the regulator to overheat, leading to unstable MCU behavior.
The issue became more pronounced after introducing multi-threading in the firmware, which increased the system's current draw. At that point, the ASM1117 began to overheat severely, risking unpredictable system failures and potential board damage.
