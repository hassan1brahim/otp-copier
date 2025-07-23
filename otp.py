import rumps        # Used to create a macOS menu bar app with a UI and timer
import sqlite3      # For reading from the iMessages database
import os           # To construct and expand file paths (like ~)
import re           # For matching OTPs using regular expressions
import pyperclip    # To copy the OTP directly to the clipboard

# Full path to the macOS iMessages database
CHAT_DB = os.path.expanduser("~/Library/Messages/chat.db")  

# Regex matches 4 to 6 digit numbers surrounded by word boundaries
# \d matches digits, {4,6} means between 4 and 6 digits
# \b ensures it doesn't match longer numbers or partial matches
OTP_REGEX = re.compile(r'\b\d{4,6}\b')  

class OTPCopierApp(rumps.App):  
    def __init__(self):
        # Create the menu bar app titled "OTP" with a built-in Quit button
        super().__init__("OTP", quit_button="Quit")  

        self.last_otp = None  # Tracks the last OTP to avoid re-copying the same one

        # Create a repeating timer that runs check_otp every 2 seconds
        self.timer = rumps.Timer(self.check_otp, 2)  
        self.timer.start()

    def check_otp(self, _):
        try:
            # Connect to the messages database
            conn = sqlite3.connect(CHAT_DB)
            cursor = conn.cursor()

            # Query the last 20 messages sent to us that contain text
            cursor.execute("""
                SELECT text FROM message
                WHERE is_from_me = 0 AND text IS NOT NULL
                ORDER BY date DESC LIMIT 20
            """)
            messages = cursor.fetchall()  # List of (text,) tuples
            conn.close()  # Always close the connection to prevent locking issues

            for (text,) in messages:
                match = OTP_REGEX.search(text)  # Look for 4–6 digit OTP in the text
                if match:
                    otp = match.group(0).strip()

                    if otp == self.last_otp:
                        return  # Same OTP as last time — do nothing

                    pyperclip.copy(otp)        # Copy new OTP to clipboard
                    self.last_otp = otp        # Update last seen OTP
                    self.title = f"{otp}"      # Show it in the menu bar
                    print(f"[debug] copied new OTP: '{otp}'")
                    return  # Exit after copying the first valid OTP

            return  # No OTP found — exit silently

        except Exception as e:
            self.title = "Error"  # Show error in menu bar title
            print(f"[error] {e}")

# Run the menu bar app if this script is launched directly
if __name__ == "__main__":
    OTPCopierApp().run()

