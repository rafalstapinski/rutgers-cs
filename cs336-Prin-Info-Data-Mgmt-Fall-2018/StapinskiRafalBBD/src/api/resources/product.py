from resources.resource import Resource
from typing import List
from models.product import Product as Model


class Product(Resource):
    def get_all(self) -> List[Model]:

        connection = self.connect_db()
        cursor = connection.cursor()

        cursor.execute("SELECT * FROM products")

        products = cursor.fetchall()

        cursor.close()
        connection.close()

        return [Model(product) for product in products]

    def get_one(self, product_id: int) -> Model:

        connection = self.connect_db()
        cursor = connection.cursor()

        cursor.execute("SELECT * FROM products WHERE id = %s", params=(product_id,))

        product = cursor.fetchone()

        cursor.close()
        connection.close()

        if product is None:
            return product

        return Model(product)

    def get_liked_by(self, drinker_id: int) -> List[Model]:

        connection = self.connect_db()
        cursor = connection.cursor()

        cursor.execute(
            """SELECT * FROM products WHERE id IN
                (SELECT product_id FROM likes WHERE drinker_id = %s)""",
            params=(drinker_id,),
        )

        products = cursor.fetchall()

        cursor.close()
        connection.close()

        return [Model(product) for product in products]

    def get_sold_at(self, bar_id: int) -> List[Model]:

        connection = self.connect_db()
        cursor = connection.cursor()

        cursor.execute(
            """SELECT * FROM products WHERE id IN
                (SELECT product_id FROM sells WHERE bar_id = %s)""",
            params=(bar_id,),
        )

        products = cursor.fetchall()

        cursor.close()
        connection.close()

        return [Model(product) for product in products]
