import * as React from 'react'
import { GetDrinker } from '../../queries/get-drinker/get-drinker'
import { RouteComponentProps } from 'react-router'
import { Header } from '../../components/header/header'
import { GetTransactionsOf } from '../../queries/get-transactions-of/get-transactions-of'
import { TopRelationChart } from '../../components/top-relation-chart/top-relation-chart'
import { aggregate, Aggregate } from '../../utils/aggregate'
import { API_HOST } from '../../../server/config'
import axios from 'axios'

interface MatchParams {
  drinkerId: number
}

type StateProps = {
  aggregate: Aggregate;
  pageWidth: number;
}

interface Props extends RouteComponentProps<MatchParams> {}

export class Drinker extends React.Component<Props, StateProps> {
  updateWindowDimensions() {
    window.addEventListener('resize', this.updateWindowDimensions)
    this.setState({ pageWidth: window.innerWidth })
  }

  async componentDidMount() {
    const url =
      API_HOST + '/drinkers/' + this.props.match.params.drinkerId + '/billed'
    const result = await axios.get(url)

    console.log(result.data.billed)

    this.setState({ aggregate: aggregate(result.data.billed) })
  }

  render() {
    return (
      <>
        <Header />
        <GetDrinker id={this.props.match.params.drinkerId} />
        <GetTransactionsOf drinkerId={this.props.match.params.drinkerId} />
        {this.state &&
          this.state.aggregate && (
            <>
              <TopRelationChart
                counter={this.state.aggregate.products}
                relation={'products'}
              />
              <TopRelationChart
                counter={this.state.aggregate.bars}
                relation={'bars'}
              />
            </>
          )}
      </>
    )
  }
}
