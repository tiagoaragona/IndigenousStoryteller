# Indigenous Storyteller Device

## Project Overview

This project harnesses the power of artificial intelligence to share the rich narratives of Indigenous cultures, dynamically tailored to the user's geographic location. Utilizing an ESP32 microcontroller, the device connects online to ascertain the user's precise location through IP geolocation. This innovative approach allows us to identify the specific region, city, and country, setting the stage for a deeply localized storytelling experience.

At the heart of the storytelling engine is the OpenAI API, which leverages the capabilities of ChatGPT to generate authentic stories from the Indigenous culture pertinent to the detected area. This unique blend of cutting-edge technology and ancient narratives offers users an educational and immersive cultural experience, bridging past and present.

## Technical Details

- **Microcontroller**: ESP32, selected for its robust WiFi capabilities and computational strength.
- **Display**: A LiquidCrystal_I2C LCD screen, providing clear and engaging displays of localized stories alongside relevant geographical information.
- **User Interface**: The design incorporates a straightforward button and LED system; a red LED signals that the device is processing, while a green LED indicates story readiness.
- **Software Integration**: Comprehensive integration of essential libraries enables seamless WiFi connectivity, HTTPS communication, JSON parsing, and LCD display management, creating a cohesive operational flow from hardware to software.
- **AI Integration**: By employing the OpenAI API, the device crafts a geolocation-based prompt to retrieve culturally relevant stories from the corresponding Indigenous traditions. The system is meticulously designed for error handling and user-friendly interactions.

## Implementation

Upon activation, the device establishes a connection with a pre-set WiFi network. Once connected, it accurately determines and displays the current geolocation. With the simple act of pressing a button, users prompt the device to request a story tied to the local Indigenous culture from the OpenAI API, which is then artfully displayed on the LCD, transforming the act of storytelling into an interactive, enlightening, and captivating experience.

## Objective and Impact

The project's goal is to deepen appreciation and understanding of Indigenous cultures through the lens of technology. By revitalizing and sharing ancestral tales, this device not only honors the past but also connects it with the present, offering a profound and educational insight into the rich cultural heritage of each location.

## License

This code is provided for educational and non-commercial use only. It should not be used for commercial purposes without permission.

## Credits

Created by Tiago Aragona. For more information, visit:
- Website: [tiagoaragona.com](https://tiagoaragona.com)
- LinkedIn: [Tiago Aragona](https://www.linkedin.com/in/tiagoaragona/)

Feel free to star and fork this project if you find it useful and inspiring!

