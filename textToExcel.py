import os
import openpyxl

def main ():
    f = open("stats.txt", "r")
    content = f.read()
    os.unlink(f.name)

    rows = []
    for row in content.split('\n'):
        rows.append(row)

    rows = rows[1:]

    # Write the values to the Excel file
    book = openpyxl.load_workbook('stats.xlsx')
    sheet = book.active
    for row in rows:
        row = row.split(", ")
        sheet.append((int(row[0]), int(row[1]), int(row[2]), int(row[3]), int(row[4]), row[5], row[6], row[7]))
        # sheet.append((int(nAsteroides), int(nIteraciones), int(nPlanetas), int(semilla), int(nIter), eType, modif, mean))
    book.save('stats.xlsx')

main()
