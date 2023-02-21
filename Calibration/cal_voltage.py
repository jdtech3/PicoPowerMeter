import pyvisa


if __name__ == '__main__':
    rm = pyvisa.ResourceManager()
    print(rm.list_resources())
