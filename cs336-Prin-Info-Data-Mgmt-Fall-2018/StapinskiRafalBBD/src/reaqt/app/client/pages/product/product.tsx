import * as React from 'react'
import { GetProduct } from '../../queries/get-product/get-product'
import { RouteComponentProps } from 'react-router'
import { Header } from '../../components/header/header'
import { Aggregate, aggregate } from '../../utils/aggregate'
import axios from 'axios'
import { API_HOST } from '../../../server/config'
import { TopRelationChart } from '../../components/top-relation-chart/top-relation-chart'

interface MatchParams {
  productId: number
}

type StateProps = {
  aggregate: Aggregate;
  pageWidth: number;
}

interface Props extends RouteComponentProps<MatchParams> {}

export class Product extends React.Component<Props, StateProps> {
  updateWindowDimensions() {
    window.addEventListener('resize', this.updateWindowDimensions)
    this.setState({ pageWidth: window.innerWidth })
  }

  async componentDidMount() {
    const url =
      API_HOST + '/products/' + this.props.match.params.productId + '/billed'
    const result = await axios.get(url)

    this.setState({ aggregate: aggregate(result.data.billed) })
  }

  render() {
    return (
      <>
        <Header />
        <GetProduct id={this.props.match.params.productId} />
        {this.state &&
          this.state.aggregate && (
            <>
              <TopRelationChart
                counter={this.state.aggregate.drinkers}
                relation={'drinkers'}
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
