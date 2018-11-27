from resources.resource import Resource


class Verify(Resource):
    def all(self):

        connection = self.connect_db()
        cursor = connection.cursor()

        cursor.execute("SELECT areAllTransactionsWithinBusinessHours()")
        areAllTransactionsWithinBusinessHours = bool(cursor.fetchone()[0])

        cursor.execute("SELECT areAllPricesConsistent()")
        areAllPricesConsistent = bool(cursor.fetchone()[0])

        cursor.execute("SELECT doAllDrinkersFrequentBarsInSameState()")
        doAllDrinkersFrequentBarsInSameState = bool(cursor.fetchone()[0])

        cursor.close()
        connection.close()

        return {
            "areAllTransactionsWithinBusinessHours": areAllTransactionsWithinBusinessHours,
            "areAllPricesConsistent": areAllPricesConsistent,
            "doAllDrinkersFrequentBarsInSameState": doAllDrinkersFrequentBarsInSameState,
        }
