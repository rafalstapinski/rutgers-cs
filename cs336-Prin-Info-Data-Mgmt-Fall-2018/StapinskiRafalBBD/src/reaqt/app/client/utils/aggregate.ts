type Billed = {
  transaction_id: number;
  bar_id: number;
  drinker_id: number;
  product_id: number;
  price: number;
  bar_name: string;
  drinker_name: string;
  product_name: string;
}

export type Counter = {
  id: number;
  x: string;
  y: number;
}

export type Aggregate = {
  drinkers: Array<Counter>;
  bars: Array<Counter>;
  products: Array<Counter>;
}

const getCounterByName = (arr: Array<Counter>, name: string): Counter => {
  return arr.find(i => i.x === name)
}

const compareByCountDesc = (a: Counter, b: Counter): number => {
  if (a.y < b.y) return 1
  if (a.y > b.y) return -1
  return 0
}

export const aggregate = (billed: Array<Billed>): Aggregate => {
  const result: Aggregate = { drinkers: [], bars: [], products: [] }

  billed.map(i => {
    const drinker = getCounterByName(result.drinkers, i.drinker_name)
    const bar = getCounterByName(result.bars, i.bar_name)
    const product = getCounterByName(result.products, i.product_name)

    if (drinker) drinker.y += 1
    else
      result.drinkers.push({
        id: i.drinker_id,
        x: i.drinker_name,
        y: 1
      })

    if (bar) bar.y += 1
    else
      result.bars.push({
        id: i.bar_id,
        x: i.bar_name,
        y: 1
      })

    if (product) product.y += 1
    else
      result.products.push({
        id: i.product_id,
        x: i.product_name,
        y: 1
      })
  })

  result.drinkers.sort(compareByCountDesc)
  result.bars.sort(compareByCountDesc)
  result.products.sort(compareByCountDesc)

  return result
}
