# Plotter_acq_firmware
SW4STM32 projet eclipse pour le firmware de Plotter_acq\
  
Pour recompiler:\
Utiliser sw4stm32  

<p>Pour changer les options de config avec CubeMX:  
Bug sur configuration du projet:  
  Pour contourner:  
      sauvegarder les fichiers ".project" et ".cproject"  
      puis les remettre après modification via CubeMX  <p\>
  De plus:\
  Il faut déplacer "cmsis_os.h" et "cmsis_os2.h" depuis "Middlewares/Third_Party/FreeRTOS/Source/CMSIS_ROS_V2" vers "Drivers/CMSIS/Include"\
 
 Info:  
 Utilise CMSIS V2 avec Freertos.  
 Pour thread debugging sur eclispe, prendre plugin freertos nxp  
 
Hardware info:  
  MCU:stm32F103RCT6  
  uart1: GPS UART  
  uart2: VHF UART  
  uart3: Pilote UART  
  uart4: Wind instrument UART  
  uart5: pc UART  
