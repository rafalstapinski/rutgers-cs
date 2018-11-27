import * as React from 'react'
import { GetBar } from '../../queries/get-bar/get-bar'
import { GetTransactionsAt } from '../../queries/get-transactions-at/get-transactions-at'
import { RouteComponentProps } from 'react-router'
import { Header } from '../../components/header/header'
import axios from 'axios'
import { API_HOST } from '../../../server/config'
import { aggregate, Aggregate } from '../../utils/aggregate'
import { TopRelationChart } from '../../components/top-relation-chart/top-relation-chart'

interface MatchParams {
  barId: number
}

type StateProps = {
  aggregate: Aggregate;
  pageWidth: number;
}

interface Props extends RouteComponentProps<MatchParams> {}

export class Bar extends React.Component<Props, StateProps> {
  updateWindowDimensions() {
    window.addEventListener('resize', this.updateWindowDimensions)
    this.setState({ pageWidth: window.innerWidth })
  }

  async componentDidMount() {
    const url = API_HOST + '/bars/' + this.props.match.params.barId + '/billed'
    const result = await axios.get(url)

    console.log(result.data)

    this.setState({ aggregate: aggregate(result.data.billed) })
  }

  render() {
    return (
      <>
        <Header />
        <GetBar id={this.props.match.params.barId} />
        <GetTransactionsAt barId={this.props.match.params.barId} />

        {this.state &&
          this.state.aggregate && (
            <>
              <TopRelationChart
                counter={this.state.aggregate.drinkers}
                relation={'drinkers'}
              />
              <TopRelationChart
                counter={this.state.aggregate.products}
                relation={'products'}
              />
            </>
          )}
      </>
    )
  }
}
