## Update Overview - Power Distribution
  In this version, a critical flaw in the power distribution design has been corrected. Previously, the STM32F469IIT6 microcontroller was powered by an ASM1117-3.3 voltage regulator, stepping down from a 12V input. However, the resulting power dissipation caused the regulator to overheat, leading to unstable MCU behavior. The issue became more pronounced after introducing multi-threading in the firmware, which increased the system's current draw. At that point, the ASM1117 began to overheat severely, risking unpredictable system failures and potential board damage.

  This overheat was detected in a prior version where devices were powered by the same regulator. In an attempt to solve this issue and still use the same regulator (Because I have many in stock), I've added dedicated regulators for some devices and it worked at a certain degree. However, it was yet not possible to place the external flash memory without overheating the ASM that powers the STM32F469. Then this update aims to fix this flaw in order to be able to utilize all the intended devices and maintain the program reliability.  

## Update Overview - Touch Screen Controller
This version removes the XPT20 touchscreen controller from the design. In version 1.0 (not publicly released), issues with the touchscreen were caused by incorrect circuitry and the absence of a controller to compensate for it. The XPT20 was originally added as a workaround to assist with potential problems in reading ADC values directly—such as noise, hardware design flaws, or programming errors. However, in version 1.1, I successfully read the touchscreen using the STM32F469’s internal ADCs without needing the XPT20. As a result, this external controller has been removed in this version to simplify the design.

Version 1.1 also introduced multi-threading, with one dedicated thread continuously reading the X-axis ADC values. Based on these readings, the system calls the scan_ function, which samples multiple values to compute both X and Y coordinates. This function dynamically reconfigures the GPIOs to alternate between driving and reading different touchscreen lines. However, this dynamic GPIO switching proved problematic in a multithreaded environment. In this update, I'm testing a workaround by adding minimal extra circuitry and using an ADC multiplexer to eliminate the need for constant GPIO reconfiguration. The goal is to reduce the processing overhead and improve stability under concurrent execution.


  
## Update

| Removed Component |  New Component     | Datasheet|
| ---------------:  | ---------------:   | ---------|
|AMS1117-3.3 |LM2576T-ADJ/NOPB  | https://www.ti.com/general/docs/suppproductinfo.tsp?distId=10&gotoUrl=https%3A%2F%2Fwww.ti.com%2Flit%2Fgpn%2Flm2576hv|
| XPT20 | N/A | https://www.alldatasheet.com/html-pdf/1649104/ETC2/XPT2046/111/1/XPT2046.html |
