import os
import openpyxl

def main ():
    # abrimos el archivo con las estadísticas, leemos los datos y lo eliminamos para evitar duplicidades futuras
    f = open("stats.txt", "r")
    content = f.read()
    os.unlink(f.name)

    rows = []
    for row in content.split('\n'):
        rows.append(row)
    rows = rows[1:]

    # escribimos los valores en un archivo formato xlsx
    book = openpyxl.load_workbook('stats.xlsx')
    sheet = book.active
    for row in rows:
        row = row.split(", ")
        sheet.append((int(row[0]), int(row[1]), int(row[2]), int(row[3]), int(row[4]), row[5], row[6], row[7]))
    book.save('stats.xlsx')

main()
