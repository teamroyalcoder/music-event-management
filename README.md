# Music Event Management Project

This repository contains a music event management project, designed to facilitate the management and organization of music event tickets. The project provides a comprehensive solution for managing tickets, including user registration, login, dynamic menu, ticket purchase, ticket management, ticket cancellation, ticket updates, viewing upcoming shows, viewing available seats, and updating user information. The project utilizes a text file-based database system for data storage.

## Project Description

The music event management project offers a wide range of functionality to enhance the ticket management experience. Some of the key features include:

- User registration and login system.
- Dynamic menu system for easy navigation.
- Ticket purchase, management, cancellation, and updates.
- Ability to view upcoming shows and available seats.
- User information management and updates.

## Upcoming Features

- [ ] Integration of QR code functionality for tickets.

Please note that the upcoming features listed above are currently in development and will be added to the project in future updates. Stay tuned for more updates and enhancements!

## Usage

To run the main music event management project, follow the instructions below:

1. Open the `trc.workspace` file in Code::Blocks.
2. Select the `main.c` file within the workspace.
3. Build and run the application using the Code::Blocks IDE.

## Misc Tools

The "misc" folder in this repository contains additional tools that can be used for various purposes. These tools are separate from the main music event management project and can be explored and utilized independently. Below are the available tools:

1. **File Deletion Tool**

   The file deletion tool allows you to delete files with specific extensions, ending words, or characters recursively within the specified root directory. This tool was primarily created to address the inconvenience of auto-generated `.save` files in the Code::Blocks IDE, which cluttered the project folders. It has been modified to provide more flexibility and cross-platform support.

   The file deletion tool is available in multiple formats: `.bat`, `.cmd`, `.js`, `.py`, and `.sh`. You can find these files in the "/misc" folder of the project's root directory.

   **Usage:**

   1. Open a command prompt or terminal.
   2. Navigate to the root directory of your project or the location where the file deletion tool is located.
   3. Run the appropriate script based on your platform:
      - For Windows: `rm_rf_exts.cmd` or `rm_rf_exts.bat`
      - For Linux/Mac: `rm_rf_exts.sh`
      - For JavaScript: `rm_rf_exts.js`
      - For Python: `rm_rf_exts.py`
   4. When prompted, enter the root directory where the file deletion should occur. If you want to remove files from the project's root directory, enter "../" (without quotes) to navigate one level up. If no directory is specified, the default is the current directory.
   5. When prompted, enter the file extensions, ending words, or characters to be deleted. Separate multiple values with commas. If no input is provided, the default file extension to be removed is `.save`.

   **Example Usage:**

   - To delete `.save` files in the project root folder:

     ```
     ./misc/rm_rf_exts.sh
     Root directory: ../
     File extensions to delete: (Press Enter)
     ```

   - To delete `.txt` and `.csv` files in a specific directory:
     ```
     ./misc/rm_rf_exts.sh
     Root directory: /path/to/directory
     File extensions to delete: .txt,.csv
     ```

   **Warning:** Exercise caution when using the file deletion tool, as it permanently deletes files from the specified directory and its subdirectories. Double-check the directory and file extensions before proceeding.

Please note that the "misc" folder may contain additional tools in the future, catering to various development needs and enhancing the usability of the repository.

## Feedback

We welcome any feedback or suggestions to enhance the functionality and usability of this project. If you encounter any issues or have ideas for improvement, please submit them through the provided channels.

Feel free to explore the various features and functionalities of this project. Should you have any questions or require assistance, please refer to the project documentation or reach out to the project maintainers.

## Contributing

Contributions to this project are not allowed. The project is intended for personal practice and learning purposes, and as such, external contributions are not accepted at this time. You are welcome to use the project for your own personal use, but please refrain from submitting pull requests or making modifications to the codebase.

If you have any suggestions, feedback, or bug reports, you can open an issue in the repository. However, please note that there is no guarantee that these requests will be addressed or incorporated into the project.

Thank you for your understanding.

## Manual License

This repository and project can be used by anyone with their own responsibility. No author credit is required, but it is appreciated if you mention the original author, Asaduzzaman Atik. Contributions are not allowed as this repository is intended for personal practice and learning purposes.

By using this project, you acknowledge that the author is not liable for any damages or consequences arising from its use. You are solely responsible for complying with any applicable laws and regulations.

Please note that this license is subject to change and may be updated in the future. For the complete and legally binding license terms, please refer to the [LICENSE](LICENSE) file in this repository.
