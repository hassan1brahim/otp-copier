from setuptools import setup

APP = ['otp.py']
DATA_FILES = []
OPTIONS = {
    'argv_emulation': False,
    'packages': ['rumps', 're', 'os', 'pyperclip'],
    'includes': ['pyperclip'],
    'plist': {
        'LSUIElement': True,
    },
}

setup(
    app=APP,
    name='OTP',
    data_files=DATA_FILES,
    options={'py2app': OPTIONS},
    setup_requires=['py2app'],
    install_requires=['rumps', 'pyperclip'],
)
