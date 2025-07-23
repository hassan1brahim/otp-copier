# OTP Copier (macOS Menu Bar App)

This is a lightweight macOS menu bar application that monitors your iMessages for one-time passcodes (OTPs) and copies the latest 4- or 6-digit code to your clipboard.

It is built using Python and the `rumps` library, and runs quietly in the menu bar, updating only when a new OTP is received.

## How it works

- Connects to the iMessage database located at `~/Library/Messages/chat.db`
- Reads the 20 most recent incoming text messages
- Uses a regular expression to detect OTP codes (4 to 6 digits)
- If a new OTP is found, it is copied to the system clipboard
- The OTP is also shown in the macOS menu bar

## Requirements

- macOS
- Python 3.9 or later
- `py2app` for packaging the app
- Python packages: `rumps`, `pyperclip`

Install the dependencies:
```
pip install rumps pyperclip
```
### Building the app

To build the standalone `.app` file:

1. Make sure `setup.py` and `otp.py` are in the same directory.
2. Run the following command:

```
python setup.py py2app
```
After the build completes, you will find the application in:
```
./dist/otp.app
```
You can run it by double-clicking the app bundle, or from the terminal using:

```
./dist/otp.app/Contents/MacOS/otp
```
### Granting Full Disk Access (Required)
macOS restricts access to the Messages database by default. You must manually grant permission:

- Open System Settings
- Go to Privacy & Security → Full Disk Access
- Click the + button
- Add the following:
- Terminal.app (if running from command line)
- The built application (otp.app)
- Python (if prompted)

Without this, the app will not be able to read your messages and will silently fail or show an error.

### Quitting the app
To exit the app, click the menu bar item and select Quit.

### License
This project is licensed under the MIT License.
