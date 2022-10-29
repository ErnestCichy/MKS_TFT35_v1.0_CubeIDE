# MKS_TFT35_v1.0_CubeIDE
stm32F407vet6.
FSMC 16bitów.

 ![mkstft35plytka](https://user-images.githubusercontent.com/115451739/198851506-df0f2cbf-dc51-4372-952d-bc34dbdcac6f.jpg)

-Projekt w Stm32CubeIDE.
Płytka MKS_TFT35 stosowana w drukarkach 3d.
MCU STM32F407vet6.
Wyświetlacz TFT 3,5" sterowany przez interfejsz równoległy FSMC 16bitów (Drivr ILI????).

Programowanie za pomocą 6-cio pinowego złącza.
Podczas programowania należy dodatkowo zasilić układ przez jedno ze złączy UART ale podając napięcie jedynie 3,3v. 
(podanie napięcia 5v na złączu UART spowoduje uruchomienie płytki ale nie pozwoli na programowanie układu!).
