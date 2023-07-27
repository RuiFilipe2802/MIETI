import PySimpleGUI as sg

def main():
    layout = [  [sg.Multiline(disabled=True,
                              autoscroll=True,
                              background_color='white',
                              size=(100,21),
                              key='chat')],
                [sg.InputText(background_color='white',
                              size=(100,1),
                              key='input',
                              focus=True)],
                [sg.Button('OK',bind_return_key=True,visible=False)]
             ]

    window = sg.Window('Chat Chat', layout, size=(640,480))
    while True:
        event, values = window.read()
        if event == sg.WIN_CLOSED:
            break
        elif event == 'OK':
            message = window['input'].get()
            if message == ":quit":
                break
            if message != "":
                window['chat'].update(message+"\n", append=True)
                window['input'].update("")
    window.close()

main()