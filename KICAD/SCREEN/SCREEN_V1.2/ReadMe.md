## Update Overview
  In this version, a critical flaw in the power distribution design has been corrected. Previously, the STM32F469IIT6 microcontroller was powered by an ASM1117-3.3 voltage regulator, stepping down from a 12V input. However, the resulting power dissipation caused the regulator to overheat, leading to unstable MCU behavior. The issue became more pronounced after introducing multi-threading in the firmware, which increased the system's current draw. At that point, the ASM1117 began to overheat severely, risking unpredictable system failures and potential board damage.

  This overheat was detected in a prior version where devices were powered by the same regulator. In an attempt to solve this issue and still use the same regulator (Because I have many in stock), I've added dedicated regulators for some devices and it worked at a certain degree. However, it was yet not possible to place the external flash memory without overheating the ASM that powers the STM32F469. Then this update aims to fix this flaw in order to be able to utilize all the intended devices and maintain the program reliability.  

## Update

| Removed Component |  New Component     | Datasheet|
| ---------------:  | ---------------:   | ---------|
|AMS1117-3.3 |  https://mm.digikey.com/Volume0/opasdata/d220001/medias/docus/6782/5272_AMS1117-3.3%20SOT-223.PDF | LM2576T-ADJ/NOPB  | https://www.ti.com/general/docs/suppproductinfo.tsp?distId=10&gotoUrl=https%3A%2F%2Fwww.ti.com%2Flit%2Fgpn%2Flm2576hv| 
