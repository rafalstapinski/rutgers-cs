import * as React from 'react'
import { BarData, BarChart } from 'react-easy-chart'
import { Counter } from '../../utils/aggregate'

type BarDataWithID = {
  id: number;
} & BarData

type OwnProps = {
  counter: Array<Counter>;
  relation: string;
}

export const TopRelationChart: React.SFC<OwnProps> = props => {
  const { counter, relation } = props

  const data = counter.length > 10 ? counter.slice(10) : counter

  console.log(counter.length, data)

  return (
    <>
      <h3>{'Top ' + relation}</h3>
      <BarChart
        axes
        colorBars
        data={counter.length > 10 ? counter.slice(0, 11) : counter}
        width={window.innerWidth}
        height={500}
        clickHandler={(d: BarDataWithID) => {
          location.href = '/' + relation + '/' + d.id
        }}
      />
    </>
  )
}
